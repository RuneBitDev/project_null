#include "../../include/game/card.h"
#include "../../include/game/card_unit.h"
#include <iostream>

card_unit::card_unit(int id, std::string name, std::string faction, int strength, bool is_hero)
    : card(id, name, faction), strength(strength), is_hero(is_hero) {}

void card_unit::display() const {
    if (this->is_hero) {
        std::cout << "Hero: " << get_name();
    } else {
        std::cout << "Unit: " << get_name();
    }
    std::cout << "(id: " << get_id() << ")" << std::endl;
}

int card_unit::get_strength() const {
    return strength;
}

void card_unit::set_strength(int strength) {
    this->strength = strength;
}

bool card_unit::get_is_hero() const {
    return is_hero;
}
