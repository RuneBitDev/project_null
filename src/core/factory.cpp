#include <iostream>
#include <fstream>
#include <sstream>
#include "sqlite3.h"
#include "game/factory.h"
#include "game/components/card.h"
#include "game/components/card_unit.h"
#include "game/components/ability/ability_summon.h"
#include "game/components/ability/ability_spy.h"
#include "game/components/ability/ability_weather.h"


bool factory::load_master_data(const std::string &filepath) {
    sqlite3* db;

    // Open the database
    if (sqlite3_open(filepath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open DB: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // LOAD ABILITIES
    const char* sql_abilities = "SELECT ability_id, name, ability_type, params FROM abilities";
    sqlite3_stmt* ab_stmt;
    if (sqlite3_prepare_v2(db, sql_abilities, -1, &ab_stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(ab_stmt) == SQLITE_ROW) {
            std::string ability_id = reinterpret_cast<const char*>(sqlite3_column_text(ab_stmt, 0));
            std::string ability_name = reinterpret_cast<const char*>(sqlite3_column_text(ab_stmt, 1));
            std::string ability_type  = reinterpret_cast<const char*>(sqlite3_column_text(ab_stmt, 2));
            std::string ability_params = reinterpret_cast<const char*>(sqlite3_column_text(ab_stmt, 3));

            std::shared_ptr<ability> new_ability = nullptr;
            std::vector<ParamValue> parsed_params = parse_params(ability_params);
            if (ability_type == "SUMMON") {
                new_ability = std::make_shared<ability_summon>(ability_id, ability_name, ability_type, parsed_params);
            } else if (ability_type == "SPY") {
                new_ability = std::make_shared<ability_spy>(ability_id, ability_name, ability_type, parsed_params);
            } else if (ability_type == "WEATHER") {
                new_ability = std::make_shared<ability_weather>(ability_id, ability_name, ability_type, parsed_params);
            }

            if (new_ability) {
                ability_library.push_back(new_ability);
            }
        }
    }
    sqlite3_finalize(ab_stmt);


    // LOAD CARDS
    const char* sql = "SELECT c.card_id, c.name, c.faction_id, c.card_type, c.rarity, "
                      "c.slots, c.is_unlocked, u.strength, u.range_type "
                      "FROM cards c "
                      "LEFT JOIN unit_stats u ON c.card_id = u.card_id";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string card_id     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string name        = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string faction_id  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string card_type   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string rarity      = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        int slots               = sqlite3_column_int(stmt, 5);
        bool is_unlocked        = sqlite3_column_int(stmt, 6) != 0;

        if (card_type == "UNIT") {
            int strength            = sqlite3_column_int(stmt, 7);
            const char* rg_ptr      = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
            std::string range_type  = rg_ptr ? rg_ptr : "SPECIAL";

            unit_library.emplace_back(card_id, name, faction_id, card_type, rarity, slots, is_unlocked, strength, range_type);
        } else {
            special_library.emplace_back(card_id, name, faction_id, card_type, rarity, slots, is_unlocked);
        }
    }
    sqlite3_finalize(stmt);


    // LINK CARD AND ABILITY
    const char* link_sql = "SELECT card_id, ability_id FROM card_abilities";
    sqlite3_stmt* link_stmt;
    if (sqlite3_prepare_v2(db, link_sql, -1, &link_stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(link_stmt) == SQLITE_ROW) {
            std::string cid = reinterpret_cast<const char*>(sqlite3_column_text(link_stmt, 0));
            std::string aid = reinterpret_cast<const char*>(sqlite3_column_text(link_stmt, 1));

            for (auto& ab : ability_library) {
                if (ab->get_id() == aid) {
                    for (auto& u : unit_library) if (u.get_id() == cid) u.add_ability(ab);
                    for (auto& s : special_library) if (s.get_id() == cid) s.add_ability(ab);
                    break;
                }
            }
        }
    }
    sqlite3_finalize(link_stmt);


    sqlite3_close(db);
    return true;

}

const std::vector<card_unit>& factory::get_unit_library() const{
    return unit_library;
}

const std::vector<card>& factory::get_special_library() const{
    return special_library;
}


deck factory::build_deck(const std::string& faction) {
    std::vector<std::unique_ptr<card>> deck_cards;
    std::unique_ptr<card> leader_ptr;
    for (const auto& unit : unit_library) {
        if (unit.get_faction_id() == faction) {
            deck_cards.push_back(unit.clone());
        }
    }

    for (const auto& special : special_library) {
        if (special.get_faction_id() == faction) {
            if (special.get_card_type() == "LEADER") {
                leader_ptr = special.clone();
            } else {
                deck_cards.push_back(special.clone());
            }
        }
    }

    return deck(std::move(leader_ptr), std::move(deck_cards));
}



// -----------------------------
// HELPER FUNCTIONS
// ---------------------------------

std::vector<ParamValue> factory::parse_params(const std::string& col_data) {
    std::vector<ParamValue> params;
    std::stringstream ss(col_data);
    std::string item;

    while (std::getline(ss, item, ',')) {
        // Trim whitespace
        item.erase(0, item.find_first_not_of(" \t\r\n"));
        item.erase(item.find_last_not_of(" \t\r\n") + 1);

        if (item.empty()) continue;

        char* end;
        long val = std::strtol(item.c_str(), &end, 10);

        if (*end == '\0') {
            params.push_back(static_cast<int>(val));
        } else {
            params.push_back(item);
        }
    }
    return params;
}