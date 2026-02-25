#include "visual/widgets/widget_row.h"
#include "visual/layout_manager.h"
#include "visual/render_config.h"

widget_row::widget_row(row_side side, row_type type)
    : side(side), type(type) {
    switch (type) {
        case row_type::MELEE:   label = "MELEE";    break;
        case row_type::RANGED:  label = "RANGED";   break;
        case row_type::HEAVY:   label = "HEAVY";    break;
        case row_type::NET:     label = "NET";      break;
        case row_type::SPECIAL: label = "SPECIAL";  break;
        default:                label = "UNKNOWN";  break;
    }
}

void widget_row::update_row(const board &game_board, widget_manager& manager) {
    if (type == row_type::SPECIAL) return;
    const auto& row_cards = game_board.get_row_cards(side, type);

    current_score = game_board.calculate_row_score(side, type);

    card_location ref_loc { side, type, 0 };
    Rectangle ref_rect = layout_manager::get_card_bounds(ref_loc, 1);

    float row_width = render_config::board::BOARD_WIDTH;
    float row_x = render_config::board::START_X;

    // handle net row
    if (static_cast<int>(type) == 3) {
        has_score = false;
        row_x += row_width + 10.0f;
        row_width /= 4.0f;
    }

    row_bounds = { row_x - 10, ref_rect.y - 5, row_width, render_config::card::CARD_HEIGHT + 10 };

    card_view_ptrs.clear();
    for (size_t i = 0; i < row_cards.size(); i++) {
        // get the persistent widget from the manager
        card_context card_ctx;

        if (auto* unit = dynamic_cast<card_unit*>(row_cards[i].get())) {
            card_ctx.virtual_strength = unit->get_virtual_strength(game_board, side, type);
            card_ctx.strength = unit->get_strength();
            card_ctx.armor = unit->get_armor();
            card_ctx.attack = unit->get_attack();
        }

        card_ctx.face_up = true;
        card_ctx.card_bounds = layout_manager::get_card_bounds({side, type, (int)i}, row_cards.size());
        card_ctx.position = card_position::ROW;

        widget_card* visual = manager.manage_card_widget(row_cards[i].get(), card_ctx);

        card_view_ptrs.push_back(visual);
    }
}

void widget_row::update(float dt) {
    for (auto& card : card_view_ptrs) {
        card->update(dt);
    }
}


void widget_row::draw() const {

    DrawRectangleRec(row_bounds, Fade(BLACK, 0.4f));
    DrawRectangleLinesEx(row_bounds, 2, Fade(BLACK, 0.5f));

    if (has_score) {
        DrawText(std::to_string(current_score).c_str(), row_bounds.x - 40, row_bounds.y, 20, DARKGREEN);
    }

    for (const auto& card_view : card_view_ptrs) {
        card_view->draw();
    }

}
