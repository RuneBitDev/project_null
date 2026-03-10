#include "visual/layout_manager.h"
#include "visual/render_config.h"

Rectangle layout_manager::get_card_bounds(card_location l, int total_cards) {
    namespace cfg = render_config;
    float w = cfg::card::CARD_WIDTH;
    float h = cfg::card::CARD_HEIGHT;
    float spacing = cfg::card::CARD_SPACING;

    auto total_cards_f = static_cast<float>(total_cards);

    // horizontal container
    float container_width = cfg::board::ROW_WIDTH;
    float container_start_x = cfg::board::START_X;

    int type_idx = static_cast<int>(l.type);

    // special row
    if (type_idx == 4) {
        container_start_x = cfg::board::START_SPECIAL_X;
        container_width = cfg::board::BOARD_SPECIAL_WIDTH;
    }
    // net row
    else if (type_idx == 3) {
        container_start_x += container_width;
        container_width /= 4.0f;
    }

    // center cards
    float container_center_x = container_start_x + (container_width / 2.0f);
    float total_row_width = (total_cards_f * w) + ((total_cards_f - 1) * spacing);
    float row_start_x = container_center_x - (total_row_width / 2.0f);
    float x = row_start_x + (static_cast<float>(l.index) * (w + spacing));

    // vertical positioning
    float y = 0.0f;
    if (type_idx == 4) {
        y = cfg::board::START_SPECIAL_Y ; // true horizon
    } else {
        float vertical_direction = (l.side == row_side::PLAYER) ? 1.0f : -1.0f;
        float horizon_offset = cfg::board::CENTER_Y_SPACING / 2.0f;

        float visual_slot = (type_idx >= 2) ? 2 : static_cast<float>(type_idx);
        float row_stack_offset = visual_slot * (h + cfg::board::ROW_SPACING);

        y = cfg::board::START_Y + (horizon_offset + row_stack_offset) * vertical_direction;

        if (l.side == row_side::OPPONENT) {
            y -= h;
        }
    }

    return { x, y, w, h };
}

Rectangle layout_manager::get_hand_card_bounds(int index, int total_cards) {
    namespace cfg = render_config;

    float w = cfg::card::CARD_WIDTH;
    float h = cfg::card::CARD_HEIGHT;
    float max_width = cfg::board::ROW_WIDTH;
    float preferred_spacing = cfg::card::CARD_SPACING;

    auto total_cards_f = static_cast<float>(total_cards);

    float natural_width = (total_cards_f * w) + ((total_cards_f - 1) * preferred_spacing);

    float actual_spacing = preferred_spacing;
    float total_w = natural_width;

    // if overflow, calculate
    if (natural_width > max_width) {
        actual_spacing = (max_width - (total_cards_f * w)) / (total_cards_f - 1);
        total_w = max_width;
    }

    // center horizontally
    float start_x = cfg::board::START_X + (cfg::board::ROW_WIDTH / 2.0f) - (total_w / 2.0f);

    float x = start_x + (static_cast<float>(index) * (w + actual_spacing));
    float y = cfg::hand::Y_OFFSET + 30.0f;

    return { x, y, w, h };
}

Rectangle layout_manager::get_graveyard_card_bounds(int index, row_side side) {
    namespace cfg = render_config;

    float w = cfg::card::CARD_WIDTH;
    float h = cfg::card::CARD_HEIGHT;

    float x = cfg::graveyard::GY_X;

    float board_center_y = cfg::board::START_Y;
    float horizon_offset = cfg::board::CENTER_Y_SPACING / 2.0f;

    float row_stack_offset = 2 * (h + cfg::board::ROW_SPACING);
    float vertical_direction = (side == row_side::PLAYER) ? 1.0f : -1.0f;

    float y = board_center_y + (horizon_offset + row_stack_offset) * vertical_direction;

    if (side == row_side::OPPONENT) {
        y -= h;
    }

    auto visual_index = static_cast<float>(std::min(index, 15));
    float stack_offset = visual_index * cfg::graveyard::GY_SPACING;

    return { x + stack_offset, y + stack_offset, w, h };
}
