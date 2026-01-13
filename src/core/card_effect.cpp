#include "../../include/game/card.h"
#include "../../include/game/card_effect.h"

#include <iostream>

card_effect::card_effect(int id, std::string name, std::string effect)
    : card(id, name, "Effect"), effect(effect) {}

std::string card_effect::get_effect() const {
    return effect;
}

void card_effect::display() const {
    std::cout << "!!! " << get_effect() << std::endl;
}
