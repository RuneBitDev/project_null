#include "visual/widgets/widget_special_row.h"
#include "visual/layout_manager.h"
#include "visual/render_config.h"

void widget_special_row::update_from_game(const board &game_board) {
    auto row_specials = game_board.get_all_specials();
    int total_cards = static_cast<int>(row_specials.size());


    card_location ref_loc { row_side::PLAYER, row_type::SPECIAL, 0 };
    Rectangle ref_rect = layout_manager::get_card_bounds(ref_loc, 1);

    float row_width = render_config::board::BOARD_SPECIAL_WIDTH;
    float row_x = render_config::board::START_SPECIAL_X;

    bounds = { row_x - 10, ref_rect.y - 5, row_width, render_config::card::CARD_HEIGHT + 10 };

    active_specials.clear();
    for (int i = 0; i < total_cards; i++) {
        card_location loc { row_side::PLAYER, row_type::SPECIAL, i };
        Rectangle c_bounds = layout_manager::get_card_bounds(loc, total_cards);

        ui_card state;
        state.face_up = true;

        active_specials.emplace_back(row_specials[i], c_bounds, state);
    }
}

void widget_special_row::update() {
    for (auto& card_widget : active_specials) {
        card_widget.update();
    }
}

void widget_special_row::draw() const {

    DrawRectangleLinesEx(bounds, 2, DARKGREEN);

    for (const auto& card_widget : active_specials) {
        card_widget.draw();
    }
}