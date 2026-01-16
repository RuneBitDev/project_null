#include <iostream>
#include <vector>
#include "game/factory.h"

int main() {
    std::cout << "--- Initializing Card Engine ---" << std::endl;

    factory game_factory;

    // Using your relative path
    if (game_factory.load_master_data("../data.sqlite")) {
        std::cout << "Database loaded successfully!\n" << std::endl;

        std::cout << "=== CARD LIBRARY (Special/Leaders) ===" << std::endl;
        // We use a getter or make the vectors public/friends to access them
        // Assuming you add a getter like get_cards() or make vectors public for this test:
        for (const auto& c : game_factory.get_card_library()) {
            c.display();
        }

        std::cout << "\n=== UNIT LIBRARY ===" << std::endl;
        for (const auto& u : game_factory.get_unit_library()) {
            u.display();
        }

    } else {
        std::cerr << "CRITICAL ERROR: Could not load data.sqlite" << std::endl;
        return 1;
    }

    return 0;
}