#include "../../../../include/visual/widgets/match/widget_graveyard.h"
#include "core/components/card_unit.h"
#include "visual/layout_manager.h"
#include "visual/render_config.h"

void widget_graveyard::update_from_player(const player &player, widget_manager_game& manager) {
    const auto& graveyard = player.get_graveyard();
    int total_cards = static_cast<int>(graveyard.size());
    row_side side = player.get_side();

    Rectangle base_card = layout_manager::get_graveyard_card_bounds(0, side);

    float padding = 20.0f;
    graveyard_bounds = {
        base_card.x - padding / 2.0f,
        base_card.y - padding / 2.0f,
        base_card.width + padding,
        base_card.height + padding
    };

    graveyard_view_ptrs.clear();
    for (int i = 0; i < total_cards; i++) {
        const card* logic_ptr = graveyard[i].get();
        Rectangle target_bounds = layout_manager::get_graveyard_card_bounds(i, side);

        card_context ctx;
        ctx.card_bounds = target_bounds;
        ctx.face_up = false;
        ctx.border_color = GRAY;
        ctx.position = card_position::GRAVEYARD;

        widget_card* visual = manager.manage_card_widget(logic_ptr, ctx);

        graveyard_view_ptrs.push_back(visual);
    }
}


void widget_graveyard::update(float dt) {
    for (auto& card_view : graveyard_view_ptrs) {
        card_view->update(dt);
    }
}

void widget_graveyard::draw() const {
    bool is_opponent = (graveyard_bounds.y < 600.0f);
    Color themeColor = is_opponent ? RED : GREEN;

    DrawRectangleRec(graveyard_bounds, Fade(BLACK, 0.6f));
    DrawRectangleLinesEx(graveyard_bounds, 2.0f, Fade(themeColor, 0.4f));

    // label
    if (!is_opponent) {
        const char* statusText = "MIKOSHI";
        DrawText(statusText, graveyard_bounds.x, graveyard_bounds.y - 20, 15, Fade(themeColor, 0.7f));
    }


    // large x
    float centerX = graveyard_bounds.x + graveyard_bounds.width / 2.0f;
    float centerY = graveyard_bounds.y + graveyard_bounds.height / 2.0f;
    float iconSize = 40.0f;

    DrawLineEx({centerX - iconSize, centerY - iconSize}, {centerX + iconSize, centerY + iconSize}, 3.0f, Fade(themeColor, 0.2f));
    DrawLineEx({centerX + iconSize, centerY - iconSize}, {centerX - iconSize, centerY + iconSize}, 3.0f, Fade(themeColor, 0.2f));

    // 5. Card Count
    std::string count = std::to_string(graveyard_view_ptrs.size());
    DrawText(count.c_str(), graveyard_bounds.x - 15, graveyard_bounds.y + 5, 20, themeColor);
}

