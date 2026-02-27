#include "visual/widgets/widget_hand.h"
#include "visual/layout_manager.h"
#include "core/components/card_unit.h"
#include "visual/render_config.h"

void widget_hand::update_from_player(const player& p, widget_manager& manager) {
    const auto& hand = p.get_hand();
    int total_cards = static_cast<int>(hand.size());

    float padding = 20.0f;
    float box_h = render_config::card::CARD_HEIGHT + padding;
    float box_y = render_config::hand::Y_OFFSET - padding / 2.0f;

    float box_w = render_config::board::BOARD_WIDTH + padding;
    float box_x = render_config::board::START_X - padding / 2.0f;

    hand_bounds = { box_x, box_y, box_w, box_h };

    card_view_ptrs.clear();
    for (int i = 0; i < total_cards; i++) {
        const card* logic_ptr = hand[i].get();

        card_context ctx;
        ctx.face_up = true;
        ctx.position = card_position::HAND;
        ctx.card_bounds = layout_manager::get_hand_card_bounds(i, total_cards);

        if (auto* unit = dynamic_cast<card_unit*>(hand[i].get())) {
            ctx.strength = unit->get_strength();
            ctx.armor = unit->get_armor();
            ctx.attack = unit->get_attack();
        }

        widget_card* visual = manager.manage_card_widget(logic_ptr, ctx);

        visual->set_bounds(ctx.card_bounds);

        card_view_ptrs.push_back(visual);
    }
}

void widget_hand::update_from_player_opponent(const player &p, widget_manager &manager) {
    const auto& hand_cards = p.get_hand();

    for (size_t i = 0; i < hand_cards.size(); i++) {
        card* c = hand_cards[i].get();

        card_context ctx;
        ctx.card_bounds = layout_manager::get_hand_card_bounds(i, hand_cards.size());
        ctx.card_bounds.y = -150.0f;
        ctx.position = card_position::HAND;
        ctx.face_up = false;

        manager.manage_card_widget(c, ctx);
    }
}

void widget_hand::update(float dt) {
    for (auto& card_view : card_view_ptrs) {
        card_view->update(dt);
    }
}

void widget_hand::draw() const {

    DrawRectangleRec(hand_bounds, Fade(BLACK, 0.4f));
    DrawRectangleLinesEx(hand_bounds, 2, Fade(BLACK, 0.5f));
}