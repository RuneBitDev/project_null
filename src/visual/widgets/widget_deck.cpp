#include "visual/widgets/widget_deck.h"
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

void widget_deck::draw() const {
    DrawRectangleRec(deck_bounds, Fade(BLACK, 0.4f));
    DrawRectangleLinesEx(deck_bounds, 2, Fade(BLACK, 0.5f));

    if (card_count <= 0) return;
    DrawText(TextFormat("%d", card_count), deck_bounds.x + 40, deck_bounds.y + 60, 20, WHITE);


}

void widget_deck::update(float dt) {
}

