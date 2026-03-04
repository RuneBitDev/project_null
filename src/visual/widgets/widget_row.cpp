#include "visual/widgets/widget_row.h"

#include <cmath>

#include "visual/layout_manager.h"
#include "visual/render_config.h"

widget_row::widget_row(row_side side, row_type type)
    : type(type), side(side) {
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

    float row_width = render_config::board::ROW_WIDTH;
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
        card_ctx.card_bounds = layout_manager::get_card_bounds({side, type, static_cast<int>(i)}, row_cards.size());
        card_ctx.position = card_position::ROW;

        widget_card* visual = manager.manage_card_widget(row_cards[i].get(), card_ctx);

        card_view_ptrs.push_back(visual);
    }
}

void widget_row::update(float dt) {
}


void widget_row::draw() const {
    Color themeColor = (side == row_side::PLAYER) ? GREEN : RED;
    float x = row_bounds.x;
    float y = row_bounds.y;
    float w = row_bounds.width;
    float h = row_bounds.height;
    // row background
    DrawRectangleGradientH(x, y, w, h, Fade(themeColor, 0.15f), Fade(BLACK, 0.0f));

    // tech-borders
    float lineLen = 30.0f;
    DrawLineEx({x, y}, {x + lineLen, y}, 2, themeColor);
    DrawLineEx({x, y}, {x, y + lineLen}, 2, themeColor);
    DrawLineEx({x, y + h}, {x + lineLen, y + h}, 2, themeColor);
    DrawLineEx({x, y + h}, {x, y + h - lineLen}, 2, themeColor);

    float pulse = (sinf(GetTime() * 1.5f) * 0.5f + 0.5f);
    // Pulse the opacity between 0.1 and 0.5
    float pulseOpacity = 0.1f + (pulse * 0.4f);

    DrawRectangleLinesEx(row_bounds, 1.5f, Fade(themeColor, pulseOpacity));


    // score banner
    if (has_score) {
        Rectangle scoreTab = { x - 60, y + (h / 2) - 20, 50, 40 };
        DrawRectangleRec(scoreTab, Fade(themeColor, 0.2f));
        DrawRectangleLinesEx(scoreTab, 1, themeColor);

        std::string scoreStr = std::to_string(current_score);
        int textW = MeasureText(scoreStr.c_str(), 24);
        DrawText(scoreStr.c_str(), scoreTab.x + (scoreTab.width/2 - textW/2), scoreTab.y + 8, 24, RAYWHITE);

        DrawText(label.c_str(), scoreTab.x, scoreTab.y - 15, 12, themeColor);
    }
}