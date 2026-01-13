#include <iostream>

#include "../include/game/factory.h"

int main() {
    std::cout << "Initiliasing!" << std::endl;
    factory factory;
    if (factory.load_master_data("../data/cards.json")) {
        std::cout << "Loading sucessfull!" << std::endl;
    } else {
        std::cout << "Failed to load!" << std::endl;
    }

    return 0;
}
