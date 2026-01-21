#include <iostream>
#include "game/engine/game_state.h"
#include "visual/render_config.h"
#include "visual/ui_element.h"

game_state::game_state(player player1, player player2)
    : p1(std::move(player1)), p2(std::move(player2)), current_round(1) {}

void game_state::handle_input(state_manager &manager) {
    ui_element ui;

    float x_offset = render_config::hand::X_OFFSET;
    const auto& hand = p1.get_hand();

    for (int i = 0; i < hand.size(); i++) {
        Rectangle card_rect = {
            x_offset,
            render_config::hand::Y_OFFSET,
            render_config::card::CARD_WIDTH,
            render_config::card::CARD_HEIGHT
        };

        if (ui.card_rec(card_rect)) {
            p1.play_card(i, game_board);
            break;
        }
        x_offset += render_config::card::CARD_WIDTH + 10;

    }
}

void game_state::update(float dt) {

}

void game_state::render(renderer& renderer) {
    renderer.draw_game(game_board, p1, p2);

}