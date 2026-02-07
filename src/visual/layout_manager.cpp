#include "visual/layout_manager.h"

#include "visual/render_config.h"

Rectangle layout_manager::get_card_bounds(row_side side, row_type type, int index, int total_cards) {
    // yeah no clue yet
    float x = 100.0f + (index * (render_config::card::CARD_WIDTH + 10));
    float y = (side == row_side::PLAYER) ? 500.0f : 200.0f;

    return { x, y, render_config::card::CARD_WIDTH, render_config::card::CARD_HEIGHT };
}
