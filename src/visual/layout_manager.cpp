#include "visual/layout_manager.h"
#include "visual/render_config.h"

Rectangle layout_manager::get_card_bounds(row_side side, row_type type, int index, int total_cards) {
    using namespace render_config;
    float w = card::CARD_WIDTH;
    float h = card::CARD_HEIGHT;
    float spacing = card::CARD_SPACING;

    // determine horizontal container
    float container_width = board::BOARD_WIDTH;
    float container_start_x = board::START_X;

    int type_idx = static_cast<int>(type);

    // the split of row 2 and 3
    if (type_idx >= 2) {
        container_width = (board::BOARD_WIDTH - 20.0f) / 2.0f;

        if (type_idx == 3) {
            container_start_x = board::START_X + container_width + 10.0f;
        }
    }

    // center cards
    float container_center_x = container_start_x + (container_width / 2.0f);
    float total_row_width = (total_cards * w) + ((total_cards - 1) * spacing);
    float row_start_x = container_center_x - (total_row_width / 2.0f);
    float x = row_start_x + (index * (w + spacing));

    // vertical positioning
    float vertical_direction = (side == row_side::PLAYER) ? 1.0f : -1.0f;
    float horizon_offset = board::CENTER_Y_SPACING / 2.0f;
    int visual_slot = (type_idx >= 2) ? 2 : type_idx;
    float row_stack_offset = visual_slot * (h + board::ROW_SPACING);

    float y = board::START_Y + (horizon_offset + row_stack_offset) * vertical_direction;

    if (side == row_side::OPPONENT) {
        y -= h;
    }

    return { x, y, w, h };
}