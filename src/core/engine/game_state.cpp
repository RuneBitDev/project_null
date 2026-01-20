#include "game/engine/game_state.h"
#include <iostream>
#include <ostream>

game_state::game_state(player player1, player player2)
    : p1(std::move(player1)), p2(std::move(player2)), current_round(1) {}

void game_state::handle_input(state_manager &manager) {

}

void game_state::update(float dt) {

}

void game_state::render(renderer& renderer) {
    renderer.draw_game(game_board, p1, p2);

}