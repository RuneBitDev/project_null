#include "game/board.h"
#include <iostream>

board::board() {
}

void board::add_card(std::unique_ptr<card> c, row_side side, row_type type) {
    int i = static_cast<int>(side);
    int j = static_cast<int>(type);

    if (i < 2 && j < 4) {
        rows[i][j].push_back(std::move(c));
    }
}

const std::vector<std::unique_ptr<card>>& board::get_row_cards(int side, int type) const {
    return rows[side][type];
}


int board::calculate_row_score(row_side side, row_type type) const {
    int score = 0;
    int s = static_cast<int>(side);
    int t = static_cast<int>(type);

    for (const auto& card_ptr : rows[s][t]) {
        card_unit* unit = dynamic_cast<card_unit*>(card_ptr.get());

        if (unit) {
            score += unit->get_strength();
        }
               // replace with get_current_strength later (buffs/debuffs)
    }

    return score;
}

int board::calculate_total_score(row_side side) const {
    int total_score = 0;

    for (int i = 0; i < 4; ++i) {
        total_score += calculate_row_score(side, static_cast<row_type>(i));
    }

    return total_score;
}

std::string board::get_row_name(row_type type) const{
    switch (type) {
        case row_type::MELEE:  return "MELEE";
        case row_type::RANGED: return "RANGED";
        case row_type::HEAVY:  return "HEAVY";
        case row_type::NET:    return "NET";
        default:               return "UNKNOWN";
    }
}
