import streamlit as st
import sqlite3
import os

st.set_page_config(page_title="Card Batch Manager", layout="wide")
st.title("🗑️ Batch Card & Asset Manager")

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DB_PATH = os.path.join(BASE_DIR, "../../data.sqlite")
PROJECT_ROOT = os.path.join(BASE_DIR, "../../")

conn = sqlite3.connect(DB_PATH, check_same_thread=False)
conn.execute("PRAGMA foreign_keys = ON;")
cursor = conn.cursor()

# 1. Fetch current data
query = """
        SELECT c.card_id, c.name, c.card_type, a.filepath, a.asset_id
        FROM cards c
                 LEFT JOIN card_assets ca ON c.card_id = ca.card_id
                 LEFT JOIN assets a ON ca.asset_id = a.asset_id
        """
cursor.execute(query)
rows = cursor.fetchall()

st.subheader("Card Library")
if rows:
    data = []
    for r in rows:
        data.append({
            "Delete": False,
            "ID": r[0],
            "Name": r[1],
            "Type": r[2],
            "Path": r[3],
            "Asset_ID": r[4]
        })

    # data editor for selection
    edited_df = st.data_editor(
        data,
        column_config={
            "Delete": st.column_config.CheckboxColumn(
                "Delete?",
                help="Select cards to be purged",
                default=False,
            )
        },
        disabled=["ID", "Name", "Type", "Path"],
        hide_index=True,
    )

    # filter for selected rows
    selected_cards = [row for row in edited_df if row["Delete"]]

    if selected_cards:
        st.warning(f"You have selected {len(selected_cards)} cards for deletion.")

        if st.button(f"🔥 Confirm: Purge {len(selected_cards)} Cards"):
            try:
                for card in selected_cards:
                    t_id = card["ID"]
                    t_path = card["Path"]
                    t_asset_id = card["Asset_ID"]

                    if t_path:
                        full_path = os.path.normpath(os.path.join(PROJECT_ROOT, t_path))
                        if os.path.exists(full_path):
                            os.remove(full_path)

                    # manual cleanup because cascade is being "special"
                    cursor.execute("DELETE FROM unit_stats WHERE card_id = ?", (t_id,))
                    cursor.execute("DELETE FROM card_abilities WHERE card_id = ?", (t_id,))
                    cursor.execute("DELETE FROM card_assets WHERE card_id = ?", (t_id,))
                    cursor.execute("DELETE FROM deck_cards WHERE card_id = ?", (t_id,))

                    if t_asset_id:
                        cursor.execute("DELETE FROM assets WHERE asset_id = ?", (t_asset_id,))

                    cursor.execute("DELETE FROM cards WHERE card_id = ?", (t_id,))

                conn.commit()
                st.success(f"Successfully purged {len(selected_cards)} cards.")
                st.rerun()

            except Exception as e:
                conn.rollback()
                st.error(f"Batch delete failed: {e}")
    else:
        st.info("Check the 'Delete' boxes above to select cards.")
else:
    st.write("No cards found in database.")