import streamlit as st
import sqlite3
import os

st.set_page_config(page_title="Card & Ability Creator", layout="wide")
st.title("🃏 Card & Ability Creator")

# --- Setup ---
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DB_PATH = os.path.join(BASE_DIR, "../../data.sqlite")
ASSETS_DIR = os.path.join(BASE_DIR, "../../data/textures/cards")

os.makedirs(ASSETS_DIR, exist_ok=True)
conn = sqlite3.connect(DB_PATH, check_same_thread=False)
cursor = conn.cursor()

# --- Global Selection (Outside Form for Reactivity) ---
choice = st.sidebar.selectbox("What are you creating?", ["Card", "Ability"])

# ---------------------------------------------------------
# CARD CREATION LOGIC
# ---------------------------------------------------------
if choice == "Card":
    st.header("Create New Card")
    card_type = st.selectbox("Type", ["UNIT", "SPECIAL", "LEADER", "SUPPORT"])

    with st.form("card_form"):
        col1, col2 = st.columns(2)

        with col1:
            card_id = st.text_input("Card ID)")
            card_name = st.text_input("Card Name")
            faction_id = st.selectbox("Faction ID", ["neutral", "arasaka", "maelstrom", "voodoo_boys", "aldecaldos", "barghest", "afterlife", "nusa"])
            rarity = st.selectbox("Rarity", ["Common", "Uncommon", "Rare", "Epic", "Legendary"])

        if card_type == "UNIT":
            with col2:
                strength = st.number_input("Power", value=0, step=1, max_value=15)
                range_type = st.selectbox("Range", ["MELEE", "RANGED", "HEAVY", "NET"])
                armor = st.number_input("Armor", value=0)
                attack = st.number_input("Attack", value=0)
        else:
            with col2:
                st.info(f"Stats are disabled for {card_type} cards.")

                ref_art = st.file_uploader("Upload Card Art (PNG)", type=['png'])

        if card_type == "UNIT":
            ref_art = st.file_uploader("Upload Card Art (PNG)", type=['png'])

        submitted_card = st.form_submit_button("Save Card to Database")

    if submitted_card:
        if not card_id or not card_name:
            st.error("Card ID and Name are required.")
        elif ref_art is None:
            st.error("Please upload card art.")
        else:
            try:
                # file saving
                file_name = f"{card_id}.png"
                full_path = os.path.join(ASSETS_DIR, file_name)
                with open(full_path, "wb") as f:
                    f.write(ref_art.getbuffer())

                # database insertion
                db_relative_path = f"data/textures/cards/{file_name}"
                asset_id = f"tex_{card_id}"

                cursor.execute("""INSERT INTO cards (card_id, name, faction_id, card_type, rarity, is_unlocked)
                                  VALUES (?, ?, ?, ?, ?, 1)""", (card_id, card_name, faction_id, card_type, rarity))

                if card_type == "UNIT":
                    cursor.execute("""INSERT INTO unit_stats (card_id, strength, range_type, armor, attack)
                                      VALUES (?, ?, ?, ?, ?)""", (card_id, strength, range_type, armor, attack))

                cursor.execute("INSERT INTO assets (asset_id, filepath, asset_type) VALUES (?, ?, ?)",
                               (asset_id, db_relative_path, "CARD_ART"))
                cursor.execute("INSERT INTO card_assets (asset_id, card_id) VALUES (?, ?)", (asset_id, card_id))

                conn.commit()
                st.success(f"Successfully created Card: {card_name}")
            except sqlite3.Error as e:
                st.error(f"Database error: {e}")

# ---------------------------------------------------------
# ABILITY CREATION LOGIC
# ---------------------------------------------------------
elif choice == "Ability":
    st.header("Create New Ability")

    ability_id = st.text_input("Ability ID (e.g., abil_strike_fire)")
    ability_name = st.text_input("Ability Name")
    ability_type = st.selectbox("Ability Type", ["MODIFIER", "SUMMON", "SPY", "STRIKE"])

    with st.form("ability_form"):
        st.subheader(f"Parameters for {ability_type}")

        if ability_type == "SUMMON":
            s_type = st.selectbox("Summon Type", ["SUMMON", "NECRO", "REINFORCE"])
            s_ids = st.text_input("Card IDs (comma separated)", help="e.g. c_01,c_02")
            params = f"{s_type}:{s_ids}"

        elif ability_type == "MODIFIER":
            m_type = st.selectbox("Mod Type", ["SET", "ADD", "SUB", "CLEAR"])
            m_row = st.selectbox("Row Target", ["MELEE", "RANGED", "HEAVY", "ALL"])
            m_target = st.selectbox("Stat Target", ["STRENGTH", "ARMOR", "ATTACK"])
            m_val = st.number_input("Value", step=1)
            params = f"{m_type},{m_row},{m_target},{m_val}"

        elif ability_type == "SPY":
            draw_amt = st.number_input("Amount to Draw", 1, 5, 1)
            params = str(draw_amt)

        elif ability_type == "STRIKE":
            st_type = st.selectbox("Strike Type", ["LETHAL", "SPLASH", "FRAG", "BARRAGE", "BREAKER"])
            st_target = st.selectbox("Target Logic", ["MAX", "MIN", "RANDOM"])
            st_val_type = st.selectbox("Value Type", ["FLAT", "PERCENT"])
            st_val = st.number_input("Value", step=1)
            params = f"{st_type},{st_target},{st_val_type},{st_val}"

        st.info(f"Generated Param String: `{params}`")
        submitted_abil = st.form_submit_button("Save Ability")

    if submitted_abil:
        if not ability_id:
            st.error("Ability ID is required.")
        else:
            try:
                cursor.execute("""INSERT INTO abilities (ability_id, name, ability_type, params)
                                  VALUES (?, ?, ?, ?)""", (ability_id, ability_name, ability_type, params))
                conn.commit()
                st.success(f"Successfully created Ability: {ability_name}")
            except sqlite3.Error as e:
                st.error(f"Database error: {e}")