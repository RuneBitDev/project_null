#include <iostream>
#include "game/factory.h"
#include "game/player.h"
#include "game/engine/game_state.h"
#include "game/engine/state_manager.h"

int main() {
    state_manager manager;
    factory game_factory;

    if (!game_factory.load_master_data("../data.sqlite")) {
        std::cerr << "CRITICAL ERROR: Could not load data.sqlite" << std::endl;
        return 1;
    }

    player p1("Arasaka", game_factory.build_deck("arasaka"));
    player p2("Barghest", game_factory.build_deck("barghest"));

    manager.push_state(std::make_unique<game_state>(std::move(p1), std::move(p2)));

    while (true) {
        manager.render();
        manager.handle_input();

    }

    return 0;
}