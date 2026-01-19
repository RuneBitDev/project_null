#include "game/engine/game_state.h"
#include <iostream>
#include <ostream>

game_state::game_state(player player1, player player2)
    : p1(std::move(player1)), p2(std::move(player2)), current_round(1) {}

void game_state::handle_input(state_manager &manager) {
    char input;
    std::cin >> input;
    if (input == 'p') {
        int input_int;
        std::cin >> input_int;
        p1.play_card((input_int),game_board);
    }
}

void game_state::update(float dt) {

}

void game_state::render(renderer& renderer) {
    debug_display();
}

void game_state::debug_display() {
    std::cout << "\n";
    std::cout << "\n=== STATE: " << get_name() << " | ROUND: " << current_round << " ===" <<std::endl;

    game_board.display_board();

    // std::cout << "Opponent Hand Size: " << game_board.get_hand_size() << std::endl;

    p1.display_hand();

    std::cout << "\nCommands: [p] Play Card (index) | [q] Quit" << std::endl;


}