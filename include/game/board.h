#ifndef PROJECT_NULL_BOARD_H
#define PROJECT_NULL_BOARD_H
#include <vector>
#include <array>
#include "card.h"
#include "card_unit.h"

enum class row_side { PLAYER, OPPONENT };
enum class row_type { MELEE = 0, RANGED = 1, HEAVY = 2, NET = 3};

class board {
    private:
    std::array<std::array<std::vector<card_unit>, 4>, 2> rows;

    std::vector<card> active_specials;

    public:
    board();
    void display_board() const;

    void add_card(card_unit c, row_side side, row_type type);
    int calculate_row_score(row_side side, row_type type) const;
    int calculate_total_score(row_side side) const;
    void clear_board();
};

#endif //PROJECT_NULL_BOARD_H