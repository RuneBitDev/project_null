#include "visual/widgets/match/widget_hand.h"
#include "visual/managers/widget_manager_match.h"
#include <cmath>
#include "visual/layout_manager.h"
#include "core/components/card_unit.h"
#include "visual/render_config.h"

void widget_hand::update_from_player(const player& p, widget_manager_match& manager) {
    const auto& hand = p.get_hand();
    int total_cards = static_cast<int>(hand.size());

    float padding = 20.0f;
    float box_h = render_config::card::CARD_HEIGHT + padding;
    float box_y = render_config::hand::Y_OFFSET + padding ;

    float box_w = render_config::board::ROW_WIDTH + padding;
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

void widget_hand::update_from_player_opponent(const player &p, widget_manager_match& manager) {
    const auto& hand_cards = p.get_hand();

    for (size_t i = 0; i < hand_cards.size(); i++) {
        card* c = hand_cards[i].get();

        card_context ctx;
        ctx.card_bounds = layout_manager::get_hand_card_bounds(i, hand_cards.size());
        ctx.card_bounds.y = -render_config::card::CARD_HEIGHT;
        ctx.position = card_position::HAND;
        ctx.face_up = false;

        manager.manage_card_widget(c, ctx);
    }
}

void widget_hand::update(float dt) {
}

void widget_hand::draw() const {
    Color themeColor = GREEN;
    float pulse = (sinf(GetTime() * 1.5f) * 0.5f + 0.5f);
    float pulseOpacity = 0.15f + (pulse * 0.35f);

    // tray glow
    DrawRectangleGradientV(hand_bounds.x, hand_bounds.y, hand_bounds.width, hand_bounds.height,
                           Fade(themeColor, 0.05f), Fade(BLACK, 0.0f));

    // borders
    DrawRectangleLinesEx(hand_bounds, 1.5f, Fade(themeColor, pulseOpacity));
    float lineLen = 40.0f;
    float x = hand_bounds.x;
    float y = hand_bounds.y;
    float w = hand_bounds.width;
    float h = hand_bounds.height;
    DrawLineEx({x, y}, {x + lineLen, y}, 2, themeColor);
    DrawLineEx({x, y}, {x, y + lineLen}, 2, themeColor);
    DrawLineEx({x, y + h}, {x + lineLen, y + h}, 2, themeColor);
    DrawLineEx({x, y + h}, {x, y + h - lineLen}, 2, themeColor);

    // labeling
    DrawText("OPERATIVE_HAND_INTERFACE", x + 10, y - 25, 18, Fade(themeColor, 0.8f));
    int count = card_view_ptrs.size();
    Color countColor = (count >= 10) ? ORANGE : themeColor;
    DrawText(TextFormat("SLOTS_FILLED: %d/10", count), x + w - 180, y - 25, 18, countColor);
}