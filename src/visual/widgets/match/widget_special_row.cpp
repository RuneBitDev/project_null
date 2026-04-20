#include "visual/widgets/match/widget_special_row.h"
#include "visual/managers/widget_manager_match.h"

#include <cmath>

#include "visual/layout_manager.h"
#include "visual/render_config.h"

void widget_special_row::update_from_game(const board &game_board, widget_manager_match& manager) {
    auto row_specials = game_board.get_all_specials();
    int total_cards = static_cast<int>(row_specials.size());


    card_location ref_loc { row_side::PLAYER, row_type::SPECIAL, 0 };
    Rectangle ref_rect = layout_manager::get_card_bounds(ref_loc, 1);

    float row_width = render_config::board::BOARD_SPECIAL_WIDTH;
    float row_x = render_config::board::START_SPECIAL_X;

    bounds = { row_x - 10, ref_rect.y - 5, row_width, render_config::card::CARD_HEIGHT + 10 };

    special_row_ptrs.clear();
    for (int i = 0; i < total_cards; i++) {
        const card* logic_ptr = row_specials[i];

        card_location loc { row_side::PLAYER, row_type::SPECIAL, i };
        Rectangle target_bounds = layout_manager::get_card_bounds(loc, total_cards);

        card_context ctx;
        ctx.face_up = true;
        ctx.card_bounds = target_bounds;
        ctx.border_color = PURPLE;

        widget_card* visual = manager.manage_card_widget(logic_ptr, ctx);

        special_row_ptrs.push_back(visual);
    }
}

void widget_special_row::update(float dt) {
}

void widget_special_row::draw() const {

    Color themeColor = PURPLE;
    DrawRectangleGradientH(bounds.x, bounds.y, bounds.width, bounds.height,
                           Fade(themeColor, 0.2f), Fade(BLACK, 0.0f));

    // brackets
    float thickness = 2.0f;
    DrawLineEx({bounds.x, bounds.y}, {bounds.x, bounds.y + bounds.height}, thickness, themeColor);
    DrawLineEx({bounds.x, bounds.y}, {bounds.x + 20, bounds.y}, thickness, themeColor);
    DrawLineEx({bounds.x, bounds.y + bounds.height}, {bounds.x + 20, bounds.y + bounds.height}, thickness, themeColor);

    // text
    DrawText("GLOBAL_MODIFIERS", bounds.x + 10, bounds.y - 20, 15, themeColor);

    // pulse
    float pulse = (sinf(GetTime() * 1.5f) * 0.5f + 0.5f) * 0.3f;
    DrawRectangleLinesEx(bounds, 1, Fade(themeColor, pulse));
}