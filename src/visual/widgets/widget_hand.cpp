#include "visual/widgets/widget_hand.h"
#include "visual/layout_manager.h"
#include "core/components/card_unit.h"
#include "visual/render_config.h"

void widget_hand::update_from_player(const player& p) {
    const auto& hand = p.get_hand();
    int total_cards = static_cast<int>(hand.size());

    float padding = 20.0f;
    float box_h = render_config::card::CARD_HEIGHT + padding;
    float box_y = render_config::hand::Y_OFFSET - padding / 2.0f;

    float box_w = render_config::board::BOARD_WIDTH + padding;
    float box_x = render_config::board::START_X - padding / 2.0f;

    hand_bounds = { box_x, box_y, box_w, box_h };

    card_views.clear();
    for (int i = 0; i < total_cards; i++) {
        Rectangle bounds = layout_manager::get_hand_card_bounds(i, total_cards);

        ui_card state;
        state.face_up = true;

        if (auto* unit = dynamic_cast<card_unit*>(hand[i].get())) {
            state.strength = unit->get_strength();
            state.armor = unit->get_armor();
            state.attack = unit->get_attack();
            state.border_color = WHITE;
        }

        card_views.emplace_back(hand[i].get(), bounds, state);
    }
}

void widget_hand::update() {
    for (auto& card_view : card_views) {
        card_view.update();
    }
}

void widget_hand::draw() const {

    DrawRectangleRec(hand_bounds, Fade(BLACK, 0.4f));
    DrawRectangleLinesEx(hand_bounds, 2, Fade(BLACK, 0.5f));

    for (const auto& card_view : card_views) {
        card_view.draw();
    }
}