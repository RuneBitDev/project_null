#include "game/card.h"

#include <iostream>
#include <ostream>
#include <string>

card::card(std::string c_card_id, std::string c_name, std::string c_faction_id, std::string c_card_type,
    std::string c_rarity, int c_slots, bool c_is_unlocked)
    : card_id(c_card_id), name(c_name), faction_id(c_faction_id), card_type(c_card_type),
    rarity(c_rarity), slots(c_slots), is_unlocked(c_is_unlocked) {}

void card::display() const {
    std::cout << "---------" << std::endl;
    std::cout << "- " << get_id() << std::endl;
    std::cout << "- " << get_name() << std::endl;
    std::cout << "- " << get_faction_id() << std::endl;
    std::cout << "- " << get_card_type() << std::endl;
    std::cout << "- " << get_rarity() << std::endl;
    std::cout << "- " << get_slots() << std::endl;
    std::cout << "- " << get_is_unlocked() << std::endl;
    std::cout << "---------" << std::endl;

}



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
