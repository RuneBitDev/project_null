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

void widget_row::update_row(const board &game_board) {
    if (type == row_type::SPECIAL) return;
    const auto& row_cards = game_board.get_row_cards(side, type);
    int total_cards = static_cast<int>(row_cards.size());

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

    card_views.clear();
    for (int i = 0; i < total_cards; i++) {
        card_location loc { side, type, i };
        Rectangle bounds = layout_manager::get_card_bounds(loc, total_cards);

        ui_card state;
        state.face_up = true;

        auto* unit = dynamic_cast<card_unit*>(row_cards[i].get());
        if (unit) {
            state.strength = unit->get_virtual_strength(game_board, side, type);
            state.armor = unit->get_armor();
            state.attack = unit->get_attack();
        }

        card_views.emplace_back(row_cards[i].get(), bounds, state);
    }
}

void widget_row::update() {
    for (auto& card : card_views) {
        card.update();
    }
}


void widget_row::draw() const {

    DrawRectangleRec(row_bounds, Fade(BLACK, 0.4f));
    DrawRectangleLinesEx(row_bounds, 2, Fade(BLACK, 0.5f));

    if (has_score) {
        DrawText(std::to_string(current_score).c_str(), row_bounds.x - 40, row_bounds.y, 20, DARKGREEN);
    }

    for (const auto& card_view : card_views) {
        card_view.draw();
    }

}
