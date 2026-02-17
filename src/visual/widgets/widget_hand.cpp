#include "visual/widgets/widget_hand.h"
#include "visual/layout_manager.h"
#include "core/components/card_unit.h"

void widget_hand::update_from_player(const player& p) {
    const auto& hand = p.get_hand();
    int total_cards = static_cast<int>(hand.size());

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
    for (const auto& card_view : card_views) {
        card_view.draw();
    }
}