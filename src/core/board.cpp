#include "game/board.h"
#include <iostream>

board::board() {
}

void board::add_card(card_unit c, row_side side, row_type type) {
    int i = static_cast<int>(side);
    int j = static_cast<int>(type);

    if (i < 2 && j < 4) {
        rows[i][j].push_back(std::move(c));
    }
}

void board::display_board() const {
    std::cout << "\n================= BOARD =================\n";

    for (int i = 1; i >= 0; --i) {
        std::string side_name = (i == 1) ? "OPPONENT" : "PLAYER";
        std::cout << "--- " << side_name << " (Total: " <<calculate_total_score(static_cast<row_side>(i)) << ") ---\n";

        for (int j = 3; j >= 0; --j) {
            std::cout << "Row [" << i << "]: ";
            if (rows[i][j].empty()) {
                std::cout << "[ Empty ]";
            } else {
                for (const auto& unit : rows[i][j]) {
                    std::cout << "[" << unit.get_name() << " (" << unit.get_strength() << ")] ";
                }
            }
            std::cout << " | SCORE: " <<calculate_row_score(static_cast<row_side>(i), static_cast<row_type>(j)) << "\n";
        }
        if (i == 1) std::cout << "-------------------------------------------\n";
    }
    std::cout << "============================================\n" << std::endl;


}

int board::calculate_row_score(row_side side, row_type type) const {
    int score = 0;
    int s = static_cast<int>(side);
    int t = static_cast<int>(type);

    for (const auto& unit : rows[s][t]) {
        score += unit.get_strength();       // replace with get_current_strength later (buffs/debuffs)
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
