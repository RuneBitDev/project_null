#include <iostream>
#include <fstream>
#include "sqlite3.h"
#include "game/factory.h"
#include "game/card.h"
#include "game/card_unit.h"


bool factory::load_master_data(const std::string &filepath) {
    sqlite3* db;

    // Open the database
    if (sqlite3_open(filepath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open DB: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

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
            card_library.emplace_back(card_id, name, faction_id, card_type, rarity, slots, is_unlocked);
        }
    }


    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;

}


std::vector<card_unit>& factory::get_unit_library() {
    return unit_library;
}

std::vector<card>& factory::get_card_library() {
    return card_library;
}
