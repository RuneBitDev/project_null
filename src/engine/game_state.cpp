#include "engine/game_state.h"
#include "visual/render_config.h"
#include "visual/ui_element.h"

game_state::game_state(player player1, player player2) {
    match = std::make_unique<match_manager>(std::move(player1), std::move(player2));
}

void game_state::handle_input(state_manager &manager) {
    ui_element::update_button(render_config::ui::PASS_BUTTON);

    if (render_config::ui::PASS_BUTTON.triggered) {
        match->pass_turn(row_side::PLAYER);
        return;
    }

    match->handle_input();
}

void game_state::update(float dt) {
    match->update(dt);
}

void game_state::render(renderer& renderer) {
    render_context ctx {
        match->get_board(),
        match->get_player(row_side::PLAYER),
        match->get_player(row_side::OPPONENT)
    };

    renderer.draw_game(ctx);
}