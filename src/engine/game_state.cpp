#include "engine/game_state.h"
#include <iostream>
#include "visual/render_config.h"


game_state::game_state(player player1, player player2) {
    match = std::make_unique<match_manager>(std::move(player1), std::move(player2));
    background = LoadTexture("data/textures/backgrounds/bckg_arasaka_01.png");
}

game_state::~game_state() {
    UnloadTexture(background);
}

void game_state::handle_input(state_manager &manager) {
    if (is_pass_button_pressed) {
        match->pass_turn(row_side::PLAYER);
        is_pass_button_pressed = false;
        return;
    }
    match->handle_input();
}

void game_state::update(float dt, renderer& renderer) {
    if (!widgets_initialized) {
        renderer.init_match_widgets(match->get_player(row_side::PLAYER),
                                    match->get_player(row_side::OPPONENT));
        widgets_initialized = true;
    }

    renderer.update_widgets(dt);

    if (renderer.is_button_triggered("PASS")) {
        is_pass_button_pressed = true;
    }

    auto status = match->update(dt);

    if (status.has_value()) {
        switch (status->game_status) {
            case game_status::CONTINUE:
                switch (status->round_status) {
                    case round_status::WIN:  renderer.add_popup("ROUND WON", GREEN, 5.0f, popup_type::BANNER);   break;
                    case round_status::LOSS: renderer.add_popup("ROUND LOST", RED, 5.0f, popup_type::BANNER);    break;
                    case round_status::DRAW: renderer.add_popup("ROUND DRAW", GRAY, 5.0f, popup_type::BANNER);   break;

                } break;
            case game_status::WIN:
                renderer.add_popup("MISSION SUCCESS", LIME, 5.0f, popup_type::BANNER);
                break;
            case game_status::LOSS:
                renderer.add_popup("SYSTEM CRITICAL: DEFEAT", RED, 5.0f, popup_type::BANNER);
                break;
            case game_status::DRAW:
                renderer.add_popup("MUTUAL DESTRUCTION", GRAY, 5.0f, popup_type::BANNER);
                break;
        }
    }


}

void game_state::render(renderer& renderer) {
    DrawTexture(background, 0, 0, WHITE);

    render_context ctx {
        match->get_board(),
        match->get_player(row_side::PLAYER),
        match->get_player(row_side::OPPONENT),
        match->get_current_state()
    };

    renderer.draw_game(ctx);
}