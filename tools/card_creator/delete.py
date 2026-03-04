import streamlit as st
import sqlite3
import os

st.set_page_config(page_title="Card Deleter", layout="wide")
st.title("🗑Card & Asset Manager")

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DB_PATH = os.path.join(BASE_DIR, "../../data.sqlite")
PROJECT_ROOT = os.path.join(BASE_DIR, "../../")

conn = sqlite3.connect(DB_PATH, check_same_thread=False)
cursor = conn.cursor()

# fetch all cards for the overview
query = """
        SELECT c.card_id, c.name, c.card_type, a.filepath, a.asset_id
        FROM cards c
                 LEFT JOIN card_assets ca ON c.card_id = ca.card_id
                 LEFT JOIN assets a ON ca.asset_id = a.asset_id \
        """
cursor.execute(query)
rows = cursor.fetchall()

st.subheader("Current Card Library")
if rows:

    card_list = [{"ID": r[0], "Name": r[1], "Type": r[2], "Path": r[3]} for r in rows]
    st.table(card_list)

    # selection for deletion
    target_id = st.selectbox("Select Card ID to Delete", [r[0] for r in rows])

    target_data = next(r for r in rows if r[0] == target_id)
    target_path = target_data[3]
    target_asset_id = target_data[4]

    if st.button(f"🔥 Permanently Delete {target_id}"):
        try:
            conn.execute("PRAGMA foreign_keys = ON;")

            cursor.execute("""
                           SELECT a.asset_id, a.filepath
                           FROM assets a
                                    JOIN card_assets ca ON a.asset_id = ca.asset_id
                           WHERE ca.card_id = ?
                           """, (target_id,))
            asset_info = cursor.fetchone() # Assuming 1:1 for now

            # manual cleanup because cascade was being "special"
            cursor.execute("DELETE FROM unit_stats WHERE card_id = ?", (target_id,))
            cursor.execute("DELETE FROM card_abilities WHERE card_id = ?", (target_id,))
            cursor.execute("DELETE FROM card_assets WHERE card_id = ?", (target_id,))
            cursor.execute("DELETE FROM deck_cards WHERE card_id = ?", (target_id,))

            # delete the asset and file
            if asset_info:
                asset_id, filepath = asset_info
                cursor.execute("DELETE FROM assets WHERE asset_id = ?", (asset_id,))

                full_path = os.path.normpath(os.path.join(PROJECT_ROOT, filepath))
                if os.path.exists(full_path):
                    os.remove(full_path)
                    st.info(f"File removed: {filepath}")

            # finally, delete the Card
            cursor.execute("DELETE FROM cards WHERE card_id = ?", (target_id,))

            conn.commit()
            st.success(f"Purged {target_id} manually.")
            st.rerun()

        except Exception as e:
            conn.rollback()
            st.error(f"Error: {e}")
else:
    st.write("No cards found in database.")