#include "game/card.h"
#include "game/card_unit.h"
#include <iostream>

card_unit::card_unit(std::string card_id, std::string name, std::string faction_id, std::string card_type,
        std::string rarity, int slots, bool is_unlocked, int strength, std::string range_type)
    : card(card_id, name, faction_id, card_type, rarity, slots, is_unlocked),
        strength(strength), range_type(range_type) {}

void card_unit::display() const {
    std::cout << "---UNIT---" << std::endl;
    std::cout << "- " << get_id() << std::endl;
    std::cout << "- " << get_name() << std::endl;
    std::cout << "- " << get_faction_id() << std::endl;
    std::cout << "- " << get_strength() << std::endl;
    std::cout << "- " << get_rarity() << std::endl;
    std::cout << "- " << get_slots() << std::endl;
    std::cout << "- " << get_is_unlocked() << std::endl;
    std::cout << "---------" << std::endl;
}


// Getter
int card_unit::get_strength() const {
    return strength;
}

std::string card_unit::get_range_type() const {
    return range_type;
}

// Setter
void card_unit::set_strength(int new_strength) {
    strength = new_strength;
}

void card_unit::set_range_type(std::string new_range_type) {
    range_type = new_range_type;
}
