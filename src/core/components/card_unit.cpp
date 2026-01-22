#include "../../../include/game/components/card.h"
#include "game/components/card_unit.h"
#include <iostream>
#include <utility>

card_unit::card_unit(std::string card_id, std::string name, std::string faction_id, std::string card_type,
        std::string rarity, int slots, bool is_unlocked, int strength, std::string range_type)
    : card(std::move(card_id), std::move(name), std::move(faction_id), std::move(card_type), std::move(rarity), slots, is_unlocked),
        strength(strength), range_type(std::move(range_type)) {}


std::unique_ptr<card> card_unit::clone() const {
    return std::make_unique<card_unit>(*this);
}


int card_unit::get_strength() const {
    return strength;
}

std::string card_unit::get_range_type() const {
    return range_type;
}


void card_unit::set_strength(int new_strength) {
    strength = new_strength;
}

void card_unit::set_range_type(std::string new_range_type) {
    range_type = std::move(new_range_type);
}
