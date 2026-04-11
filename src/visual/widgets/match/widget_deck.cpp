#include "../../../../include/visual/widgets/match/widget_deck.h"

#include <cmath>

#include "visual/render_config.h"

void widget_deck::init_deck(row_side side) {
    deck_bounds = (side == row_side::PLAYER) ? render_config::deck::DECK_PLAYER : render_config::deck::DECK_OPPONENT;
    this->side = side;
}

void widget_deck::update_from_deck(const deck &d, widget_manager &manager) {
    this-> card_count = d.get_size();

    for (card* c : d.get_card_ptrs()) {
        card_context card_ctx;
        card_ctx.card_bounds = deck_bounds;
        card_ctx.position = card_position::DECK;
        card_ctx.face_up = false;
        manager.manage_card_widget(c, card_ctx);
    }
}

void widget_deck::update(float dt) {
}

void widget_deck::draw() const {
    Color themeColor = (side == row_side::PLAYER) ? GREEN : RED;

    DrawRectangleRec(deck_bounds, Fade(BLACK, 0.8f));
    DrawRectangleLinesEx(deck_bounds, 2.0f, themeColor);

    // scanning bar
    float scanY = deck_bounds.y + (deck_bounds.height * (sinf(GetTime() * 2.0f) * 0.5f + 0.5f));
    DrawLineEx({deck_bounds.x, scanY}, {deck_bounds.x + deck_bounds.width, scanY}, 1.0f, Fade(themeColor, 0.4f));

    if (card_count <= 0) {
        DrawText("DEPLETED", deck_bounds.x + 10, deck_bounds.y + deck_bounds.height/2 - 10, 15, RED);
        return;
    }

    // card count
    Rectangle badge = { deck_bounds.x - 15, deck_bounds.y - 15, 45, 45 };
    DrawCircleV({badge.x + 22, badge.y + 22}, 22, BLACK);
    DrawCircleLines(badge.x + 22, badge.y + 22, 22, themeColor);

    std::string countStr = std::to_string(card_count);
    int textWidth = MeasureText(countStr.c_str(), 20);
    DrawText(countStr.c_str(), badge.x + 22 - textWidth/2, badge.y + 12, 20, RAYWHITE);
}

