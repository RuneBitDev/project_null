#include "game/components/card.h"
#include <iostream>
#include <ostream>
#include <string>
#include <utility>

card::card(std::string c_card_id, std::string c_name, std::string c_faction_id, std::string c_card_type,
    std::string c_rarity, int c_slots, bool c_is_unlocked)
    : card_id(std::move(c_card_id)), name(std::move(c_name)), faction_id(std::move(c_faction_id)), card_type(std::move(c_card_type)),
    rarity(std::move(c_rarity)), slots(c_slots), is_unlocked(c_is_unlocked) {}



std::unique_ptr<card> card::clone() const {
    return std::make_unique<card>(*this);
}

// bool card::set_ability(std::shared_ptr<ability> ability) {
//     if (slots >= 1) {
//         card_abilities.push_back(ability);
//         return true;
//     }
//     return false;
// }


// -----------------------------------------
// Getter
// -----------------------------------------

std::string card::get_id() const {
    return card_id;
}

std::string card::get_name() const {
    return name;
}

std::string card::get_faction_id() const {
    return faction_id;
}

std::string card::get_card_type() const {
    return card_type;
}

std::string card::get_rarity() const {
    return rarity;
}

int card::get_slots() const {
    return slots;
}

bool card::get_is_unlocked() const {
    return is_unlocked;
}


// -----------------------------------------
// Setter
// -----------------------------------------

void card::set_slots(int new_slots) {
    slots = new_slots;
}

void card::set_is_unlocked(bool new_value) {
    is_unlocked = new_value;
}
