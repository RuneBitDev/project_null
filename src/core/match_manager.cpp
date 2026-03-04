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

std::optional<match_status> match_manager::update(float dt) {
    switch (current_match_state) {

        case current_state::PLAYER_TURN:
            if (p1.get_has_passed()) {
                std::cout << "[STATE] Player passed, switching turn..." << std::endl;
                switch_turn();
            }
            break;

        case current_state::AI_TURN:
            ai_timer += dt;
            if (ai_timer >= 1.5f) {
                std::cout << "[STATE] AI Timer hit, executing turn..." << std::endl;
                execute_ai_turn();
                ai_timer = 0.0f;
                switch_turn();
            }
            break;

        case current_state::FIREFIGHT:
            combat.update(dt);
            if (!combat.is_busy()) {
                std::cout << "[STATE] Firefight finished. Checking next step..." << std::endl;
                if (p1.get_has_passed() && p2.get_has_passed()) {
                    std::cout << "[STATE] Both passed -> ROUND_OVER" << std::endl;
                    current_match_state = current_state::ROUND_OVER;
                } else {
                    current_match_state = (active_player_side == row_side::PLAYER)
                                          ? current_state::PLAYER_TURN
                                          : current_state::AI_TURN;
                    std::cout << "[STATE] Returning to: " << (int)current_match_state << std::endl;
                }
            }
            break;

        case current_state::ROUND_OVER: {
            std::cout << "[STATE] Entering ROUND_OVER logic" << std::endl;
            round_status r_res = end_round();
            game_status g_res = end_game();

            if (g_res != game_status::CONTINUE) {
                current_match_state = current_state::GAME_OVER;
            } else {
                prepare_next_round();
            }
            return match_status{ r_res, g_res };
        }

        case current_state::GAME_OVER:
            return match_status{ round_status::DRAW, end_game() };
    }

    return std::nullopt;
}

void match_manager::switch_turn() {
    total_turns_this_round++;
    if (total_turns_this_round > 4) clash_counter++;

    // swap sides
    active_player_side = (active_player_side == row_side::PLAYER)
                         ? row_side::OPPONENT
                         : row_side::PLAYER;

    // validation: if active player passed -> swap back
    // unless BOTH have passed
    if (active_player_side == row_side::PLAYER && p1.get_has_passed()) {
        if (!p2.get_has_passed()) active_player_side = row_side::OPPONENT;
    }
    else if (active_player_side == row_side::OPPONENT && p2.get_has_passed()) {
        if (!p1.get_has_passed()) active_player_side = row_side::PLAYER;
    }

    // trigger firefight or set next state
    bool both_passed = (p1.get_has_passed() && p2.get_has_passed());
    bool clash_ready = (clash_counter >= 2);

    if (both_passed || clash_ready) {
        current_match_state = current_state::FIREFIGHT;
        combat.firefight();
        clash_counter = 0;
    } else {
        current_match_state = (active_player_side == row_side::PLAYER)
                              ? current_state::PLAYER_TURN
                              : current_state::AI_TURN;
    }
}

void match_manager::pass_turn(row_side side) {
    if (side == row_side::PLAYER) p1.set_has_passed(true);
    else p2.set_has_passed(true);

    if (!first_passer.has_value()) {
        first_passer = side;
    }

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
    if (current_match_state != current_state::PLAYER_TURN) return;
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
    for (int s = 0; s < 2; s++) {
        auto side = static_cast<row_side>(s);
        for (int t = 0; t < 5; t++) {
            auto type = static_cast<row_type>(t);
            const auto& row_cards = game_board.get_row_cards(side, type);
            for (int i = 0; i < static_cast<int>(row_cards.size()); i++) {
                card_location loc { static_cast<row_side>(s), static_cast<row_type>(t), i };
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


// ---------------------------- INTERNAL  ----------------------------

void match_manager::prepare_next_round() {
    std::cout << "--- PREPARING NEXT ROUND ---" << std::endl;

    if (first_passer.has_value()) {
        active_player_side = (first_passer.value() == row_side::PLAYER)
                             ? row_side::OPPONENT
                             : row_side::PLAYER;
        std::cout << "[ROUND] First passer was " << static_cast<int>(first_passer.value())
                  << ". Starter will be " << static_cast<int>(active_player_side) << std::endl;
    }

    first_passer = std::nullopt;
    p1.set_has_passed(false);
    p2.set_has_passed(false);
    total_turns_this_round = 0;
    clash_counter = 0;
    current_round++;

    game_board.clear_board(p1, p2);

    current_match_state = (active_player_side == row_side::PLAYER)
                          ? current_state::PLAYER_TURN
                          : current_state::AI_TURN;

    std::cout << "[ROUND] Round " << current_round << " started. State: "
              << static_cast<int>(current_match_state) << std::endl;
}

round_status match_manager::end_round() {
    int p1_score = get_player_score(row_side::PLAYER);
    int p2_score = get_player_score(row_side::OPPONENT);

    round_scores[row_side::PLAYER].push_back(p1_score);
    round_scores[row_side::OPPONENT].push_back(p2_score);

    if (p1_score > p2_score) {
        p2.lose_live();
        return round_status::WIN;
    } else if (p2_score > p1_score) {
        p1.lose_live();
        return round_status::LOSS;
    }
    p1.lose_live(); p2.lose_live();
    return round_status::DRAW;
}

game_status match_manager::end_game() {
    int p1_lives = p1.get_lives();
    int p2_lives = p2.get_lives();

    if (p1_lives <= 0 && p2_lives <= 0) return game_status::DRAW;
    if (p1_lives <= 0) return game_status::LOSS;
    if (p2_lives <= 0) return game_status::WIN;

    return game_status::CONTINUE;
}

void match_manager::execute_ai_turn() {
    const auto& hand = p2.get_hand();

    bool out_of_cards = hand.empty();
    bool should_pass = false;

    if (p1.get_has_passed()) {
        if (get_player_score(row_side::OPPONENT) > get_player_score(row_side::PLAYER)) {
            should_pass = true;
        }
    }

    if (out_of_cards || should_pass) {
        std::cout << "[DEBUG] AI is passing turn." << std::endl;
        p2.set_has_passed(true);
        switch_turn();
        return;
    }

    p2.play_card(0, game_board, row_side::OPPONENT, p1, combat);
}

player & match_manager::get_player(row_side side) {
    return (side == row_side::PLAYER) ? p1 : p2;
}

const player& match_manager::get_player(row_side side) const {
    return (side == row_side::PLAYER) ? p1 : p2;
}

