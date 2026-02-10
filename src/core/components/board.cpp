#include "core/components/board.h"
#include <algorithm>
#include <ranges>

// ---------------------------- MOVE ----------------------------
void board::add_card(std::unique_ptr<card> c, row_side side, row_type type) {
    int i = static_cast<int>(side);
    int j = static_cast<int>(type);

    rows[i][j].push_back(std::move(c));
}


void board::clear_board(player &p1, player &p2) {
    for (int side = 0; side < 2; ++side) {
        player& target_player = (side == 0) ? p1 : p2;

        // Iterate through all 5 row types (Melee, Ranged, Heavy, Net, Special)
        for (int type = 0; type < 5; ++type) {
            auto& current_row = rows[side][type];

            for (auto& card_ptr : current_row) {
                if (card_ptr->get_card_type() == "UNIT") {
                    target_player.add_to_graveyard(std::move(card_ptr));
                }
            }
            current_row.clear();
        }
    }

    clear_all_modifiers();
}

void board::destroy_card(const std::vector<card*>& targets, player& p1, player& p2) {
    for (int s = 0; s < 2; ++s) {
        player& target_player = (s == 0) ? p1 : p2;
        for (int t = 0; t < 5; ++t) {
            auto& row = rows[s][t];

            // pretty dope erase-if
            std::erase_if(row, [&](std::unique_ptr<card>& c_ptr) {
                auto it = std::find(targets.begin(), targets.end(), c_ptr.get());
                if (it != targets.end()) {
                    if (c_ptr->get_card_type() == "UNIT") {
                        target_player.add_to_graveyard(std::move(c_ptr));
                    }
                    return true; // erase
                }
                return false;
            });
        }
    }
}


// ---------------------------- SCORE ----------------------------
int board::calculate_row_score(row_side side, row_type type) const {
    int score = 0;
    int s = static_cast<int>(side);
    int t = static_cast<int>(type);

    for (const auto& card_ptr : rows[s][t]) {
        if (auto* unit = dynamic_cast<card_unit*>(card_ptr.get())) {
            score += unit->get_virtual_strength(*this, side, type);
        }
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

    auto get_priority = [](modifier_type t) {
        switch (t) {
            case modifier_type::SET:        return 0;
            case modifier_type::ADD:        return 1;
            case modifier_type::SUBTRACT:   return 2;
            case modifier_type::MULTIPLY:   return 3;
            default:                        return 4;
        }
    };

    std::ranges::sort(active_modifiers[key], [&](const auto& a, const auto& b) {
        return get_priority(std::get<0>(a)) < get_priority(std::get<0>(b));
    });
}

bool board::is_side_row_modified(const std::tuple<row_side, row_type> &key) const {
    auto it = active_modifiers.find(key);
    return it != active_modifiers.end();
}

std::vector<std::tuple<modifier_type, int>> board::get_modifiers(row_side side, row_type type) const {
    auto key = std::make_tuple(side, type);
    auto it = active_modifiers.find(key);

    if (it != active_modifiers.end()) {
        return it->second;
    }

    static constexpr std::vector<std::tuple<modifier_type, int>> empty_vector;
    return empty_vector;
}

void board::clear_modifier(modifier_type m_type) {
    // clean vectors
    for (auto &mod_list : active_modifiers | std::views::values) {
        std::erase_if(mod_list, [m_type](const auto& mod_tuple) {
            return std::get<0>(mod_tuple) == m_type;
        });
    }
    // clean keys that point to empty vector (better safe than sorry ey)
    std::erase_if(active_modifiers, [](const auto& pair) {
        return pair.second.empty();
    });
}

void board::clear_all_modifiers() {
    active_modifiers.clear();
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

const std::vector<std::unique_ptr<card>>& board::get_row_cards(int side, int type) const {
    return rows[side][type];
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

// Helper to get strength from a card pointer safely
int get_unit_strength(const std::unique_ptr<card>& c, value_type v_type, const board& b, int side, int type) {
    auto* unit = dynamic_cast<card_unit*>(c.get());
    if (!unit) return -1;
    switch (v_type) {
        case value_type::STRENGTH:
            return unit->get_virtual_strength(b, static_cast<row_side>(side), static_cast<row_type>(type));
        case value_type::ARMOR:
            return unit->get_armor();
        case value_type::ATTACK:
            return unit->get_attack();
        default:
            return -1;
    }
}

std::vector<card*> board::get_max_value_cards_on_board(value_type v_type) const {
    int max_val = -1;
    std::vector<card*> targets;

    // find global max
    for (int s = 0; s < 2; ++s) {
        for (int t = 0; t < 5; ++t) {
            for (const auto& c : rows[s][t]) {
                int val = get_unit_strength(c, v_type, *this, s, t);
                if (val > max_val) max_val = val;
            }
        }
    }

    // get all of them cards
    if (max_val <= 0) return targets; // or none

    for (int s = 0; s < 2; ++s) {
        for (int t = 0; t < 5; ++t) {
            for (const auto& c : rows[s][t]) {
                if (get_unit_strength(c, v_type, *this, s, t) == max_val) {
                    targets.push_back(c.get());
                }
            }
        }
    }
    return targets;
}

std::vector<card*> board::get_max_value_cards_on_row(value_type v_type, row_side side, row_type r_type) const {
    int max_val = -1;
    std::vector<card*> targets;
    const auto& row = rows[static_cast<int>(side)][static_cast<int>(r_type)];

    for (const auto& c : row) {
        int val = get_unit_strength(c, v_type, *this, static_cast<int>(side), static_cast<int>(r_type));
        if (val > max_val) max_val = val;
    }

    if (max_val <= 0) return targets;

    for (const auto& c : row) {
        if (get_unit_strength(c, v_type, *this, static_cast<int>(side), static_cast<int>(r_type)) == max_val) {
            targets.push_back(c.get());
        }
    }
    return targets;
}


