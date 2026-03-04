import streamlit as st
import sqlite3
import os



st.title(" Card Creator")

BASE_DIR = os.path.dirname(os.path.abspath(__file__))


DB_PATH = os.path.join(BASE_DIR, "../../data.sqlite")
ASSETS_DIR = os.path.join(BASE_DIR, "../../data/textures/cards")


print(f"Looking for DB at: {os.path.abspath(DB_PATH)}")

os.makedirs(ASSETS_DIR, exist_ok=True)

conn = sqlite3.connect(DB_PATH, check_same_thread=False)
cursor = conn.cursor()


choice = st.selectbox("What are you creating?", ["Card", "Ability"])
with st.form("card_data"):
    if choice == "Card":
        card_id = st.text_input("Card ID")
        card_name = st.text_input("Card Name")
        faction_id = st.selectbox("Faction ID", ["neutral", "arasaka", "maelstrom", "voodoo_boys", "aldecaldos", "barghest", "afterlife", "nusa"])
        card_type = st.selectbox("Type", ["UNIT", "SPECIAL", "LEADER"])
        rarity = st.selectbox("Rarity", ["Common", "Uncommon", "Rare", "Epic", "Legendary"])

        if card_type == "UNIT":
            strength = st.number_input("Power", value=0, step=1, max_value=15)
            range_type = st.selectbox("Range", ["MELEE", "RANGED", "HEAVY", "NET"])
            armor = st.number_input("Armor", value=0)
            attack = st.number_input("Attack", value=0)

    else:
        ability_id = st.text_input("Card ID (for Special Card)")
        ability_name = st.text_input("Special Card Name")
        ability_type = st.selectbox("Ability Type", ["MODIFIER", "SUMMON", "SPY", "STRIKE"])
        params = st.text_input("Params (JSON or string)")

    ref_art = st.file_uploader("Upload Art", type=['png'])
    submitted = st.form_submit_button("Submit Data")

if submitted:
    try:
        # handle image file
        if ref_art is not None:
            file_name = f"{card_id if choice == 'Card' else ability_id}.png"
            full_path = os.path.join(ASSETS_DIR, file_name)

            with open(full_path, "wb") as f:
                f.write(ref_art.getbuffer())

            db_relative_path = f"data/textures/cards/{file_name}"
            asset_id = f"tex_{card_id if choice == 'Card' else ability_id}"
        else:
            st.error("Please upload an image!")
            st.stop()

        # database transaction
        if choice == "Card":
            cursor.execute("""
                           INSERT INTO cards (card_id, name, faction_id, card_type, rarity, is_unlocked)
                           VALUES (?, ?, ?, ?, ?, 1)""",
                           (card_id, card_name, faction_id, card_type, rarity))

            if card_type == "UNIT":
                cursor.execute("""
                               UPDATE unit_stats
                               SET strength = ?, range_type = ?, armor = ?, attack = ?
                               WHERE card_id = ?""",
                               (strength, range_type, armor, attack, card_id))

            cursor.execute("INSERT INTO assets (asset_id, filepath, asset_type) VALUES (?, ?, ?)",
                           (asset_id, db_relative_path, "CARD_ART"))

            cursor.execute("INSERT INTO card_assets (asset_id, card_id) VALUES (?, ?)",
                           (asset_id, card_id))

        elif choice == "Ability":
            cursor.execute("""
                           INSERT INTO cards (card_id, name, faction_id, card_type, rarity, is_unlocked)
                           VALUES (?, ?, ?, 'SPECIAL', 'Common', 1)""",
                           (ability_id, ability_name, "neutral"))

            cursor.execute("INSERT INTO assets (asset_id, filepath, file_type) VALUES (?, ?, ?)",
                           (asset_id, db_relative_path, "ABILITY_ART"))

            cursor.execute("INSERT INTO card_assets (asset_id, card_id) VALUES (?, ?)",
                           (asset_id, ability_id))

        conn.commit()
        st.success(f"Successfully created {card_id if choice == 'Card' else ability_id} and linked assets!")

    except sqlite3.Error as e:
        st.error(f"Database error: {e}")
    except Exception as e:
        st.error(f"Error: {e}")