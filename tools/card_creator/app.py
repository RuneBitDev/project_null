import streamlit as st
import sqlite3



st.title(" Card Creator")

DB_PATH = "../../data.sqlite"
ASSETS_DIR = "../../data/cards/"

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

        if card_type == "UNIT" :
            strength = st.number_input("Power", value=0, step=1, max_value=15)
            range_type = st.selectbox("Range", ["MELEE", "RANGED", "HEAVY", "NET"])




    else:
        ability_id = st.text_input("Ability ID")
        ability_name = st.text_input("Ability Name")
        ability_type = st.selectbox("Type", ["EFFECT", "CYBERWARE", "WEATHER", "SUMMON", "QUICKHACK"])




    ref_art = st.file_uploader("Upload Reference Art", type=['png'])
    submitted = st.form_submit_button("Submit Data")

if submitted:
    if choice == "Card":
        cursor.execute("""
                       INSERT INTO cards (card_id, name, faction_id, card_type, rarity)
                       VALUES (?, ?, ?, ?, ?)""",
                       (card_id, card_name, faction_id.lower(), card_type, rarity)
                       )
        conn.commit()
        st.success("Card saved!")

    elif choice == "Ability":
        cursor.execute("""
                       INSERT INTO abilities (ability_id, name, ability_type)
                       VALUES (?, ?, ?)""",
                       (ability_id, ability_name, ability_type)
                       )
        conn.commit()
        st.success("Ability saved!")