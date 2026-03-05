#include "engine/game_state.h"
#include "engine/menu_state.h"
#include <iostream>

#include "engine/state_manager.h"


game_state::game_state(player player1, player player2, factory& factory, texture_factory& texture_factory)
    : data_factory(factory), tex_factory(texture_factory) {

    std::vector<std::string> cards_to_load;
    for (const auto& card : player1.get_deck().get_card_ptrs()) {
        cards_to_load.push_back(card->get_id());
    }
    for (const auto& card : player2.get_deck().get_card_ptrs()) {
        cards_to_load.push_back(card->get_id());
    }
    tex_factory.load_texture_for_cards(cards_to_load);

    match = std::make_unique<match_manager>(std::move(player1), std::move(player2));
}

game_state::~game_state() {
    tex_factory.unload_all();
}

void game_state::handle_input(state_manager &manager) {

    if (game_over && end_screen_timer <= 0.0f) {
        manager.change_state(std::make_unique<menu_state>(data_factory, tex_factory));
        std::cout << "GAME OVER" << std::endl;
        return;
    }
    if (is_pass_button_pressed) {
        match->pass_turn(row_side::PLAYER);
        is_pass_button_pressed = false;
        return;
    }
    if (!game_over) {
        match->handle_input();
    }
}

void game_state::update(float dt, renderer& renderer) {
    if (!widgets_initialized) {
        renderer.init_match_widgets(match->get_player(row_side::PLAYER), match->get_player(row_side::OPPONENT), tex_factory);
        widgets_initialized = true;
    }

    renderer.update_widgets(dt);

    if (cards_drawn < INITIAL_HAND_SIZE) {
        if (!intro_delay_finished) {
            intro_timer -= dt;
            if (intro_timer <= 0) intro_delay_finished = true;
        } else {
            draw_timer += dt;
            if (draw_timer >= TIME_BETWEEN_CARDS) {
                match->get_player(row_side::PLAYER).draw_card(1);
                match->get_player(row_side::OPPONENT).draw_card(1);

                cards_drawn++;
                draw_timer = 0.0f;
            }
        }
    }

    // pass button logic
    bool is_player_turn = (match->get_current_state() == current_state::PLAYER_TURN);
    bool already_passed = match->get_player(row_side::PLAYER).get_has_passed();
    renderer.set_button_enabled("PASS", is_player_turn && !already_passed);
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
                game_over = true;
                break;
            case game_status::LOSS:
                renderer.add_popup("SYSTEM CRITICAL: DEFEAT", RED, 5.0f, popup_type::BANNER);
                game_over = true;
                break;
            case game_status::DRAW:
                renderer.add_popup("MUTUAL DESTRUCTION", GRAY, 5.0f, popup_type::BANNER);
                game_over = true;
                break;
        }
    }

    if (game_over) {
        end_screen_timer -= dt;
    }
}

void game_state::render(renderer& renderer) {

    render_context ctx {
        match->get_board(),
        match->get_player(row_side::PLAYER),
        match->get_player(row_side::OPPONENT),
        match->get_current_state()
    };
    renderer.draw_game(ctx);
}