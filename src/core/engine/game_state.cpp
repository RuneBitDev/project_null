#include <iostream>
#include "game/engine/game_state.h"
#include "visual/render_config.h"
#include "visual/ui_element.h"

game_state::game_state(player player1, player player2)
    : p1(std::move(player1)), p2(std::move(player2)), current_round(1) {
    p1.set_side(row_side::PLAYER);
    p2.set_side(row_side::OPPONENT);
}

void game_state::handle_input(state_manager &manager) {
    ui_element ui;
    player& active_p = (active_player == 1) ? p1 : p2;

    if (active_player == 1 && !p1.get_has_passed()) {
        const auto& hand = active_p.get_hand();
        float x_offset = render_config::hand::X_OFFSET;

        for (int i = 0; i < hand.size(); i++) {
            Rectangle card_rect = {
                x_offset,
                render_config::hand::Y_OFFSET,
                render_config::card::CARD_WIDTH,
                render_config::card::CARD_HEIGHT
            };

            if (ui.card_rec(card_rect)) {
                active_p.play_card(i, game_board, row_side::PLAYER, p2);

                if (!p2.get_has_passed()) {
                    active_player = 2;
                }
                return;
            }
            x_offset += render_config::card::CARD_WIDTH + 10;

        }
    }

}

void game_state::update(float dt) {
    if (p1.get_has_passed() && p2.get_has_passed()) {
        if (!round_ended) {
            round_ended = true;
        }
        return;
    }

    if (active_player == 2 && !p2.get_has_passed()) {
        ai_timer += dt;

        if (ai_timer >= 1.5f) {
            execute_ai_turn();
            ai_timer = 0.0f;

            if (!p1.get_has_passed()) {
                active_player = 1;
            }
        }
    }

}

void game_state::render(renderer& renderer) {
    renderer.draw_game(game_board, p1, p2);

}

player& game_state::get_other_player() {
    return (active_player == 1) ? p2 : p1;
}


player& game_state::get_player(row_side side) {
    return (side == row_side::PLAYER) ? p1 : p2;
}

player& game_state::get_opponent(row_side side) {
    return (side == row_side::PLAYER) ? p2 : p1;
}


void game_state::execute_ai_turn() {
    auto& hand = p2.get_hand();

    if (hand.empty()) {
        p2.set_has_passed(true);
    } else {
        // Simple AI: Just play the first card in the vector
        p2.play_card(0, game_board, row_side::OPPONENT, p1);
    }
}