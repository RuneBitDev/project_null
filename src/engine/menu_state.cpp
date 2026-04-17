#include "engine/menu_state.h"
#include "core/factory.h"
#include "engine/construction_state.h"
#include "engine/match_state.h"
#include "engine/state_manager.h"


menu_state::menu_state(factory& game_factory, texture_factory& texture_factory)
    : data_factory(game_factory), tex_factory(texture_factory) {
    ui_manager = std::make_unique<widget_manager_menu>();
}

menu_state::~menu_state() = default;


void menu_state::handle_input(state_manager &manager) {

    if (ui_button_clicked("START")) {
        player player1("V", data_factory.load_deck("arasaka_01"));
        player player2("Opponent", data_factory.load_deck("arasaka_01"));

        manager.change_state(std::make_unique<match_state>(std::move(player1), std::move(player2), data_factory, tex_factory));
    }
    if (ui_button_clicked("DECK")) {
        manager.change_state(std::make_unique<construction_state>(data_factory, tex_factory));
    }
}

void menu_state::update(float dt, renderer& renderer) {
    ui_manager->update(dt);


    auto update_selection = [&](const std::string& prev_id, const std::string& next_id, int& index, std::string& faction) {
        if (ui_button_clicked(prev_id)) {
            index = (index - 1 + static_cast<int>(available_factions.size())) % static_cast<int>(available_factions.size());
            faction = available_factions[index];

        }
        if (ui_button_clicked(next_id)) {
            index = (index + 1) % static_cast<int>(available_factions.size());
            faction = available_factions[index];

        }
    };

    update_selection("P1_PREV", "P1_NEXT", p1_select, p1_faction);
    update_selection("P2_PREV", "P2_NEXT", p2_select, p2_faction);


}

void menu_state::render(renderer& renderer) {
    renderer.draw_menu(p1_select, p2_select, available_factions);
    ui_manager->draw(renderer);
}