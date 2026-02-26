#include "engine/game_state.h"

#include <iostream>

#include "visual/render_config.h"
#include "visual/ui_element.h"

game_state::game_state(player player1, player player2) {
    match = std::make_unique<match_manager>(std::move(player1), std::move(player2));
    background = LoadTexture("data/textures/backgrounds/bckg_arasaka_01.png");
}

game_state::~game_state() {
    UnloadTexture(background);
}

void game_state::handle_input(state_manager &manager) {
    ui_element::update_button(render_config::ui::PASS_BUTTON);

    if (render_config::ui::PASS_BUTTON.triggered) {
        match->pass_turn(row_side::PLAYER);
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

    auto result = match->update(dt);
    if (result.has_value()) {
        switch (result.value()) {
            case game_end::CONTINUE: {
                int p1_score = match->get_player_score(row_side::PLAYER);
                int p2_score = match->get_player_score(row_side::OPPONENT);

                if (p1_score > p2_score) renderer.add_popup("ROUND WON", GREEN, 5.0f, popup_type::BANNER);
                else if (p2_score > p1_score) renderer.add_popup("ROUND LOST", RED, 5.0f, popup_type::BANNER);
                else renderer.add_popup("ROUND DRAW", GRAY, 5.0f, popup_type::BANNER);
                break;
            }
            case game_end::WIN:
                renderer.add_popup("MISSION SUCCESS", LIME, 5.0f, popup_type::BANNER);
                break;
            case game_end::LOSE:
                renderer.add_popup("SYSTEM CRITICAL: DEFEAT", RED, 5.0f, popup_type::BANNER);
                break;
            case game_end::DRAW:
                renderer.add_popup("MUTUAL DESTRUCTION", GRAY, 5.0f, popup_type::BANNER);
                break;
        }
    }

    renderer.update_widgets(dt);
}

void game_state::render(renderer& renderer) {
    DrawTexture(background, 0, 0, WHITE);

    render_context ctx {
        match->get_board(),
        match->get_player(row_side::PLAYER),
        match->get_player(row_side::OPPONENT)
    };

    renderer.draw_game(ctx);
}