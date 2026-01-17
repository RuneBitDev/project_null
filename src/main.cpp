#include <iostream>
#include <vector>
#include "game/factory.h"
#include "game/deck.h"

int main() {
    std::cout << "--- Initializing Card Engine ---" << std::endl;

    factory game_factory;

    // Using your relative path
    if (game_factory.load_master_data("../data.sqlite")) {
        std::cout << "Database loaded successfully!\n" << std::endl;

        deck deck1 = game_factory.build_deck("arasaka");
        deck deck2 = game_factory.build_deck("barghest");

        for (auto& unit: deck1.get_units()) {
            unit.display();
        }

        for (auto& special: deck1.get_special()) {
            special.display();
        }

        deck1.get_leader().display();

    } else {
        std::cerr << "CRITICAL ERROR: Could not load data.sqlite" << std::endl;
        return 1;
    }

    return 0;
}