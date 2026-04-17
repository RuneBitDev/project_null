#include "engine/match_state.h"
#include "engine/menu_state.h"
#include <iostream>

#include "core/game_log.h"
#include "engine/state_manager.h"


match_state::match_state(player player1, player player2, factory& factory, texture_factory& texture_factory)
    : data_factory(factory), tex_factory(texture_factory) {

    ui_manager = std::make_unique<widget_manager_match>(player1, player2, texture_factory);

    std::vector<std::string> cards_to_load;
    for (const auto& card : player1.get_deck().get_card_ptrs()) {
        cards_to_load.push_back(card->get_id());
    }
    for (const auto& card : player2.get_deck().get_card_ptrs()) {
        cards_to_load.push_back(card->get_id());
    }
    tex_factory.load_texture_for_cards(cards_to_load);

    match = std::make_unique<match_manager>(std::move(player1), std::move(player2));

    game_log::add_break();
    game_log::add("--- ENCOUNTER INITIALIZED ---", GOLD);
    game_log::add("Match: " + match->get_player(row_side::PLAYER).get_name() + " vs " +
                  match->get_player(row_side::OPPONENT).get_name(), GRAY);
    
}

match_state::~match_state() {
    tex_factory.unload_transient();
}

void match_state::handle_input(state_manager &manager) {

    if (game_over && end_screen_timer <= 0.0f) {
        manager.change_state(std::make_unique<menu_state>(data_factory, tex_factory));
        std::cout << "GAME OVER" << std::endl;
        return;
    }
    if (ui_button_clicked("PASS")) {
        match->pass_turn(row_side::PLAYER);
        return;
    }
    if (!game_over) {
        match->handle_input();
    }
}

void match_state::update(float dt, renderer& renderer) {

    render_context ctx {
        match->get_board(),
        match->get_player(row_side::PLAYER),
        match->get_player(row_side::OPPONENT),
        match->get_current_state()
    };
    if (auto* match_ui = dynamic_cast<widget_manager_match*>(ui_manager.get())) {
        match_ui->sync_with_game(ctx);
    }
    ui_manager->update(dt);

    if (cards_drawn < INITIAL_HAND_SIZE) {
        if (!intro_delay_finished) {
            intro_timer -= dt;
            if (intro_timer <= 0) {
                intro_delay_finished = true;
                game_log::add("[SYSTEM]: Distributing initial hand...", GRAY);
            }
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

    ui_manager->set_button_enabled("PASS", is_player_turn && !already_passed);

    auto status = match->update(dt);

    if (status.has_value()) {
        switch (status->g_status) {
            case game_status::CONTINUE:
                switch (status->r_status) {
                    case round_status::WIN:
                        game_log::add(">> ROUND WON <<", GREEN);
                        ui_manager->add_popup("ROUND WON", GREEN, 5.0f, popup_type::BANNER);   break;
                    case round_status::LOSS:
                        game_log::add(">> ROUND LOST <<", RED);
                        ui_manager->add_popup("ROUND LOST", RED, 5.0f, popup_type::BANNER);    break;
                    case round_status::DRAW:
                        game_log::add(">> ROUND DRAW <<", GRAY);
                        ui_manager->add_popup("ROUND DRAW", GRAY, 5.0f, popup_type::BANNER);   break;

                } break;
            case game_status::WIN:
                game_log::add("CRITICAL SYSTEM VICTORY", LIME);
                ui_manager->add_popup("CRITICAL SYSTEM VICTORY", LIME, 5.0f, popup_type::BANNER);
                game_over = true;
                break;
            case game_status::LOSS:
                game_log::add("SYSTEM CRITICAL: DEFEAT", RED);
                ui_manager->add_popup("SYSTEM CRITICAL: DEFEAT", RED, 5.0f, popup_type::BANNER);
                game_over = true;
                break;
            case game_status::DRAW:
                game_log::add("MUTUAL DESTRUCTION", GRAY);
                ui_manager->add_popup("MUTUAL DESTRUCTION", GRAY, 5.0f, popup_type::BANNER);
                game_over = true;
                break;
        }
    }

    if (game_over) {
        end_screen_timer -= dt;
    }
}

void match_state::render(renderer& renderer) {

    renderer.draw_background();

    if (ui_manager) {
        ui_manager->draw(renderer);
    }
}