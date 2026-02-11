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

void board::remove_card_at(card_location loc, player& p) {
    if (!loc.is_valid()) return;

    auto& row = rows[static_cast<int>(loc.side)][static_cast<int>(loc.type)];

    if (loc.index >= 0 && loc.index < (int)row.size()) {
        // move to graveyard
        if (row[loc.index]->get_card_type() == "UNIT") {
            p.add_to_graveyard(std::move(row[loc.index]));
        }
        // erase from vector (unique_ptr should clear)
        row.erase(row.begin() + loc.index);
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

int get_val(const std::unique_ptr<card>& c, value_type v_type, const board& b, row_side s, row_type t) {
    if (auto* unit = dynamic_cast<card_unit*>(c.get())) {
        switch (v_type) {
            case value_type::STRENGTH: return unit->get_virtual_strength(b, s, t);
            case value_type::ARMOR:    return unit->get_armor();
            case value_type::ATTACK:   return unit->get_attack();
        }
    }
    return -1; // ignore non-units
}

std::vector<card_location> board::get_max_value_locations_on_board(value_type v_type) const {
    int max_val = -1;
    std::vector<card_location> targets;

    // get global max
    for (int s = 0; s < 2; ++s) {
        for (int t = 0; t < 5; ++t) {
            auto side = static_cast<row_side>(s);
            auto type = static_cast<row_type>(t);
            for (const auto& c : rows[s][t]) {
                max_val = std::max(max_val, get_val(c, v_type, *this, side, type));
            }
        }
    }

    if (max_val <= 0) return targets;

    // collect global max
    for (int s = 0; s < 2; ++s) {
        for (int t = 0; t < 5; ++t) {
            auto side = static_cast<row_side>(s);
            auto type = static_cast<row_type>(t);
            for (int i = 0; i < (int)rows[s][t].size(); ++i) {
                if (get_val(rows[s][t][i], v_type, *this, side, type) == max_val) {
                    targets.push_back({side, type, i});
                }
            }
        }
    }
    return targets;
}

std::vector<int> board::get_dead_unit_indices(row_side side, row_type type) const {
    std::vector<int> dead_indices;
    const auto& row = rows[static_cast<int>(side)][static_cast<int>(type)];

    for (int i = static_cast<int>(row.size()) - 1; i >= 0; --i) {
        if (auto* unit = dynamic_cast<card_unit*>(row[i].get())) {
            if (unit->is_dead()) {
                dead_indices.push_back(i);
            }
        }
    }
    return dead_indices;
}


