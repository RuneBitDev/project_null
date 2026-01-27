#include "game/components/board.h"


void board::add_card(std::unique_ptr<card> c, row_side side, row_type type) {
    int i = static_cast<int>(side);
    int j = static_cast<int>(type);

    if (is_row_weathered(type)) {
        if (auto* unit = dynamic_cast<card_unit*>(c.get())) {
            unit->set_weathered(true);
        }
    }

    rows[i][j].push_back(std::move(c));
}

const std::vector<std::unique_ptr<card>>& board::get_row_cards(int side, int type) const {
    return rows[side][type];
}

// sick Visitor Pattern
void board::for_each_card(const std::function<void(card&)>& action) {
    for (auto& side : rows) {
        for (auto& row : side) {
            for (auto& card_ptr : row) {
                if (card_ptr) action(*card_ptr);
            }
        }
    }
}

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

void board::set_row_weather(row_type type, bool active) {
    active_weather[type] = active;
}

bool board::is_row_weathered(row_type type) const {
    auto it = active_weather.find(type);
    return it != active_weather.end() && it->second;
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
