#include "visual/widgets/widget_graveyard.h"
#include "core/components/card_unit.h"
#include "visual/layout_manager.h"
#include "visual/render_config.h"

void widget_graveyard::update_from_player(const player &player) {
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

    graveyard_views.clear();
    for (int i = 0; i < total_cards; i++) {
        Rectangle bounds = layout_manager::get_graveyard_card_bounds(i, side);

        ui_card state;
        state.face_up = false;
        graveyard_views.emplace_back(graveyard[i].get(), bounds, state);
    }
}


void widget_graveyard::update() {
    for (auto& card_view : graveyard_views) {
        card_view.update();
    }
}

void widget_graveyard::draw() const {

    DrawRectangleRec(graveyard_bounds, Fade(BLACK, 0.4f));
    DrawRectangleLinesEx(graveyard_bounds, 2, Fade(BLACK, 0.5f));

    for (const auto& card_view : graveyard_views) {
        card_view.draw();
    }
}

