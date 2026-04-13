#include "engine/menu_state.h"
#include "core/factory.h"
#include "engine/construction_state.h"
#include "engine/game_state.h"
#include "engine/state_manager.h"


menu_state::menu_state(factory& game_factory, texture_factory& texture_factory)
    : data_factory(game_factory), tex_factory(texture_factory) {}

menu_state::~menu_state() = default;


void menu_state::handle_input(state_manager &manager) {

    if (start_button_is_pressed) {
        player player1("V", data_factory.load_deck("arasaka_01"));
        player player2("Opponent", data_factory.load_deck("arasaka_01"));

        manager.change_state(std::make_unique<game_state>(std::move(player1), std::move(player2), data_factory, tex_factory));
    }
    if (deck_button_is_pressed) {
        manager.change_state(std::make_unique<construction_state>(data_factory, tex_factory));
    }
}

void menu_state::update(float dt, renderer& renderer) {
    if (!widgets_initialized) {
        renderer.init_menu_widgets(available_factions, p1_faction, p2_faction);
        widgets_initialized = true;
    }

    renderer.update_match_widgets(dt);


    auto update_selection = [&](const std::string& prev_id, const std::string& next_id, int& index, std::string& faction) {
        if (renderer.is_button_triggered(prev_id)) {
            index = (index - 1 + static_cast<int>(available_factions.size())) % static_cast<int>(available_factions.size());
            faction = available_factions[index];
            widgets_initialized = false;
        }
        if (renderer.is_button_triggered(next_id)) {
            index = (index + 1) % static_cast<int>(available_factions.size());
            faction = available_factions[index];
            widgets_initialized = false;
        }
    };

    update_selection("P1_PREV", "P1_NEXT", p1_select, p1_faction);
    update_selection("P2_PREV", "P2_NEXT", p2_select, p2_faction);

    if (renderer.is_button_triggered("START")) {
        start_button_is_pressed = true;
    }
    if (renderer.is_button_triggered("DECK")) {
        deck_button_is_pressed = true;
    }
}

void menu_state::render(renderer& renderer) {
    renderer.draw_menu(p1_select, p2_select, available_factions);
}