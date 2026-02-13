#include "core/match_manager.h"
#include "visual/render_config.h"
#include "visual/layout_manager.h"
#include <iostream>
#include <numeric>

match_manager::match_manager(player player1, player player2)
    : p1(std::move(player1)),
      p2(std::move(player2)),
      active_player_side(row_side::PLAYER),
      combat(game_board, p1, p2) {
    p1.set_side(row_side::PLAYER);
    p2.set_side(row_side::OPPONENT);
}

// ---------------------------- TURN LOGIC ----------------------------

void match_manager::update(float dt) {
    if (p1.get_has_passed() && p2.get_has_passed()) {
        end_round();
        return;
    }

    if (combat.is_busy()) {
        combat.update(dt);
        return;
    }

    if (active_player_side == row_side::OPPONENT && !p2.get_has_passed()) {
        ai_timer += dt;
        if (ai_timer >= 1.5f) {
            execute_ai_turn();
            ai_timer = 0.0f;
            switch_turn();
        }
    }
}

void match_manager::switch_turn() {
    if (active_player_side == row_side::PLAYER) {
        if (!p2.get_has_passed()) active_player_side = row_side::OPPONENT;
    } else {
        if (!p1.get_has_passed()) active_player_side = row_side::PLAYER;
    }

    if (active_player_side == row_side::PLAYER || p1.get_has_passed()) {
        combat.firefight();
    }
}

void match_manager::pass_turn(row_side side) {
    if (side == row_side::PLAYER) p1.set_has_passed(true);
    else p2.set_has_passed(true);
    switch_turn();
}

// ---------------------------- ACTIONS ----------------------------

void match_manager::play_card_from_hand(int index, row_side side) {
    player& active = (side == row_side::PLAYER) ? p1 : p2;
    player& opponent = (side == row_side::PLAYER) ? p2 : p1;

    active.play_card(index, game_board, side, opponent, combat);

    combat.cleanup_dead_units();
}

void match_manager::handle_input() {

    if (combat.is_busy()) return;
    if (active_player_side != row_side::PLAYER || p1.get_has_passed()) return;

    Vector2 mouse = render_config::get_virtual_mouse();
    // hand interactions
    const auto& hand = p1.get_hand();
    for (int i = static_cast<int>(hand.size()) - 1; i >= 0; i--) {
        Rectangle bounds = layout_manager::get_hand_card_bounds(i, hand.size());
        if (CheckCollisionPointRec(mouse, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            play_card_from_hand(i, row_side::PLAYER);
            switch_turn();
            return;
        }
        if (CheckCollisionPointRec(mouse, bounds) && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            if (auto* u = dynamic_cast<card_unit*>(hand[i].get())) {
                u->change_stance();
            }
        }
    }

    // board interactions
    for (int side = 0; side < 2; side++) {
        for (int type = 0; type < 5; type++) {
            const auto& row_cards = game_board.get_row_cards(side, type);
            for (int i = 0; i < static_cast<int>(row_cards.size()); i++) {
                card_location loc { static_cast<row_side>(side), static_cast<row_type>(type), i };
                Rectangle b = layout_manager::get_card_bounds(loc, row_cards.size());
                if (CheckCollisionPointRec(mouse, b)) {
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        std::cout << "Inspecting: " << row_cards[i]->get_name() << "\n";
                    }
                    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                        if (auto* u = dynamic_cast<card_unit*>(row_cards[i].get())) {
                            u->change_stance();
                        }
                    }
                    return;
                }
            }
        }
    }
}


// ---------------------------- SCORING ----------------------------

int match_manager::get_player_score(row_side side) const {
    return game_board.calculate_total_score(side);
}

game_end match_manager::end_round() {
    int p1_score = get_player_score(row_side::PLAYER);
    int p2_score = get_player_score(row_side::OPPONENT);

    // score storing (wtf say this out loud a few times -> you get a stroke)
    round_scores[row_side::PLAYER].push_back(p1_score);
    round_scores[row_side::OPPONENT].push_back(p2_score);

    // winner winner chicken diner
    if (p1_score > p2_score) {
        p2.lose_live();
    } else if (p2_score > p1_score) {
        p1.lose_live();
    } else {
        p1.lose_live();
        p2.lose_live();
    }

    if (p1.get_lives() <= 0 && p2.get_lives() <= 0) return game_end::DRAW;
    if (p1.get_lives() <= 0) return game_end::LOSE;
    if (p2.get_lives() <= 0) return game_end::WIN;

    game_board.clear_board(p1, p2);
    p1.set_has_passed(false);
    p2.set_has_passed(false);
    current_round++;

    return game_end::CONTINUE;
}

void match_manager::execute_ai_turn() {
    const auto& hand = p2.get_hand();
    if (p1.get_has_passed()) {
        p2.set_has_passed(true);
    } else {
        p2.play_card(0, game_board, row_side::OPPONENT, p1, combat);
    }
}

const player& match_manager::get_player(row_side side) const {
    return (side == row_side::PLAYER) ? p1 : p2;
}

