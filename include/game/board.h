#ifndef PROJECT_NULL_BOARD_H
#define PROJECT_NULL_BOARD_H
#include <vector>
#include <array>
#include "card.h"
#include "card_unit.h"

enum class row_side { PLAYER, OPPONENT };
enum class row_type { MELEE = 0, RANGED = 1, HEAVY = 2, NET = 3, SPECIAL = 4};

class board {
public:
    board() = default;

    void add_card(std::unique_ptr<card> c, row_side side, row_type type);
    const std::vector<std::unique_ptr<card>>& get_row_cards(int side, int type) const;
    int calculate_row_score(row_side side, row_type type) const;
    int calculate_total_score(row_side side) const;

    std::string get_row_name(row_type type) const;
    void clear_board();

private:
    std::array<std::array<std::vector<std::unique_ptr<card>>, 4>, 2> rows;
    std::vector<std::unique_ptr<card>> active_specials;
};

#endif //PROJECT_NULL_BOARD_H