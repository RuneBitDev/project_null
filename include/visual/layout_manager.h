#ifndef PROJECT_NULL_LAYOUT_MANAGER_H
#define PROJECT_NULL_LAYOUT_MANAGER_H
#include "raylib.h"
#include "../core/types.h"

class layout_manager {
public:
    layout_manager() = delete;

    static Rectangle get_card_bounds(row_side side, row_type type, int index, int total_cards);
    static Rectangle get_hand_card_bounds(int index, int total_cards);
};

#endif //PROJECT_NULL_LAYOUT_MANAGER_H