#include "game/components/board.h"

// ---------------------------- MOVE ----------------------------
void board::add_card(std::unique_ptr<card> c, row_side side, row_type type) {
    auto key = std::make_tuple(side, type);
    int i = static_cast<int>(side);
    int j = static_cast<int>(type);

    if (is_side_row_modified(key)) {
        if (auto* unit = dynamic_cast<card_unit*>(c.get())) {
            for (auto& modifier : active_modifiers[key]) {
                modifier_type m_type = std::get<0>(modifier);
                int m_value = std::get<1>(modifier);
                unit->save_modifier(m_type, m_value);
            }
        }
    }

    rows[i][j].push_back(std::move(c));
}

const std::vector<std::unique_ptr<card>>& board::get_row_cards(int side, int type) const {
    return rows[side][type];
}


void board::clear_board(player &p1, player &p2) {
    for (int side = 0; side < 2; ++side) {
        player& target_player = (side == 0) ? p1 : p2;

        // Iterate through all 5 row types (Melee, Ranged, Heavy, Net, Special)
        for (int type = 0; type < 5; ++type) {
            auto& current_row = rows[side][type];

            for (auto& card_ptr : current_row) {
                if (card_ptr) {
                    target_player.add_to_graveyard(std::move(card_ptr));
                }
            }
            current_row.clear();
            // still need to delete saved modifiers
        }
    }
}



// ---------------------------- SCORE ----------------------------
int board::calculate_row_score(row_side side, row_type type) const {
    int score = 0;
    int s = static_cast<int>(side);
    int t = static_cast<int>(type);

    for (const auto& card_ptr : rows[s][t]) {
        score += card_ptr->get_strength();
    }

    return score;
}

int board::calculate_total_score(row_side side) const {
    int total_score = 0;

    for (int i = 0; i < 5; ++i) {
        total_score += calculate_row_score(side, static_cast<row_type>(i));
    }

    return total_score;
}
// ---------------------------- MODIFIER ----------------------------

void board::save_modifiers(row_side side, row_type r_type, modifier_type m_type, int m_value) {
    auto key = std::make_tuple(side, r_type);
    active_modifiers[key].emplace_back(m_type, m_value);
}

bool board::is_side_row_modified(std::tuple<row_side, row_type> key) const {
    auto it = active_modifiers.find(key);
    return it != active_modifiers.end();
}

std::vector<std::tuple<modifier_type, int>> board::get_modifiers(row_side side, row_type type) const {
    auto key = std::make_tuple(side, type);
    auto it = active_modifiers.find(key);

    if (it != active_modifiers.end()) {
        return it->second;
    }

    static const std::vector<std::tuple<modifier_type, int>> empty_vector;
    return empty_vector;
}


// ------------------------ GETTER & SETTER -------------------------

// sick Visitor Pattern
void board::for_each_card(const std::function<void(card&)>& action) const {
    for (auto& side : rows) {
        for (auto& row : side) {
            for (auto& card_ptr : row) {
                if (card_ptr) action(*card_ptr);
            }
        }
    }
}

std::string board::get_row_name(row_type type) const {
    switch (type) {
        case row_type::MELEE:  return "MELEE";
        case row_type::RANGED: return "RANGED";
        case row_type::HEAVY:  return "HEAVY";
        case row_type::NET:    return "NET";
        case row_type::SPECIAL: return "SPECIAL";
        default:               return "UNKNOWN";
    }
}
