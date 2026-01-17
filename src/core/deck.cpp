#include <iostream>
#include "game/deck.h"
#include "game/game_config.h"

deck::deck(card c_leader, std::vector<card> c_special_cards, std::vector<card_unit> c_unit_cards)
    : leader(c_leader), special_cards(c_special_cards), unit_cards(c_unit_cards) {}

bool deck::is_valid() {
    int unit_count = unit_cards.size();
    int special_count = special_cards.size();

    if (unit_count < game_config::deck_rules::MIN_UNIT_CARDS) {
        std::cout << "Error: Not enough unit cards!" << std::endl;
        return false;
    }

    if (special_count < game_config::deck_rules::MIN_SPECIAL_CARDS) {
        std::cout << "Error: Not enough special cards!" << std::endl;
        return false;
    }

    return true;
}

card  deck::get_leader() const{
    return leader;
}

const std::vector<card>& deck::get_special() const {
    return special_cards;
}

const std::vector<card_unit>& deck::get_units() const {
    return unit_cards;
}