#include "game/engine/game_state.h"

#include <iostream>
#include <ostream>

game_state::game_state(player player1, player player2)
    : p1(std::move(player1)), p2(std::move(player2)), current_round(1) {}

void game_state::handle_input(state_manager &manager) {
    char input;
    std::cin >> input;
    if (input == 'd') p1.draw_card();
}

void game_state::update(float dt) {

}

void game_state::render() {
    debug_display();
}

void game_state::debug_display() {
    std::cout << "\n=== ROUND: " << current_round << " ===" << std::endl;

    std::cout << "P1 Score: " << game_board.calculate_total_score(row_side::PLAYER) << std::endl;
    std::cout << "P2 Score: " << game_board.calculate_total_score(row_side::OPPONENT) << std::endl;


}

std::string game_state::get_name() const {
    return "game_state";
}
