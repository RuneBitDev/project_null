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
    ui_element::update_button(render_config::ui::PASS_BUTTON);
    player& active_p = (active_player == 1) ? p1 : p2;

    if (render_config::ui::PASS_BUTTON.triggered) {
        p1.set_has_passed(true);
        p2.set_has_passed(true);
    }

    if (active_player == 1 && !p1.get_has_passed()) {
        const auto& hand = active_p.get_hand();
        float x_offset = render_config::hand::X_OFFSET;

        for (int i = 0; i < hand.size(); i++) {
            ui_card card(x_offset, render_config::hand::Y_OFFSET, true);
            ui_element::update_card(card);
            if (card.triggered) {
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
        switch (end_round()) {
            case game_end::CONTINUE:
                break;
            case game_end::WIN:
                // WIN
            case game_end::LOSE:
                // LOSE
            case game_end::DRAW:
                // DRAW
            default:
                std::cout << "\nTHIS SHOULD NEVER HAPPEN!!!";
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
    render_context ctx { game_board, p1, p2 };
    renderer.draw_game(ctx);

}

// ---------------------------------------
// HELPER FUNCTIONS
// ---------------------------------------

game_end game_state::end_round() {
    current_round++;

    // Calculate Winner and store values
    int round_score_player = game_board.calculate_total_score(row_side::PLAYER);
    int round_score_opp = game_board.calculate_total_score(row_side::OPPONENT);
    int end_of_round_score = round_score_player - round_score_opp;
    std::cout << "\nSCORE: " << end_of_round_score << std::endl;

    if (end_of_round_score > 0) {
        end_of_round_state = round_state::WIN;
        p2.lose_live();
    } else if (end_of_round_score < 0) {
        end_of_round_state = round_state::LOSE;
        p1.lose_live();
    } else {
        end_of_round_state = round_state::DRAW;
        p1.lose_live();
        p2.lose_live();
    }

    round_scores[row_side::PLAYER].push_back(round_score_player);
    round_scores[row_side::OPPONENT].push_back(round_score_opp);

    if (p1.get_lives() == 0 && p2.get_lives() == 0) {
        return  game_end::LOSE;
    } else if (p2.get_lives() == 0) {
        return game_end::WIN;
    } else if (p2.get_lives() == 0 ) {
        return game_end::DRAW;
    }

    // Reset for next round
    game_board.clear_board(p1, p2);
    p1.set_has_passed(false);
    p2.set_has_passed(false);
    return game_end::CONTINUE;


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