#include "engine/menu_state.h"
#include "core/factory.h"
#include "engine/game_state.h"
#include "engine/state_manager.h"


menu_state::menu_state(factory& game_factory, texture_factory& texture_factory)
    : data_factory(game_factory), tex_factory(texture_factory) {}


void menu_state::handle_input(state_manager &manager) {
    if (show_start_screen) {
        if (IsKeyPressed(KEY_ENTER)) show_start_screen = false;
    }

    if (start_button_is_pressed) {
        player player1("V", data_factory.build_deck(p1_faction));
        player player2("Opponent", data_factory.build_deck(p2_faction));

        manager.change_state(std::make_unique<game_state>(std::move(player1), std::move(player2), data_factory, tex_factory));
    }
}

void menu_state::update(float dt, renderer& renderer) {
    if (!widgets_initialized) {
        renderer.init_menu_widgets(available_factions, p1_faction, p2_faction);
        widgets_initialized = true;
    }

    renderer.update_widgets(dt);


    for (const auto& faction : available_factions) {
        if (renderer.is_button_triggered("P1_" + faction)) {
            p1_faction = faction;
            widgets_initialized = false;
        }
        if (renderer.is_button_triggered("P2_" + faction)) {
            p2_faction = faction;
            widgets_initialized = false;
        }
    }

    if (!show_start_screen && renderer.is_button_triggered("START")) {
        start_button_is_pressed = true;
    }
}

void menu_state::render(renderer& renderer) {
    if (show_start_screen) {
        renderer.draw_start_screen();
    } else {
        renderer.draw_menu();

    }
}
