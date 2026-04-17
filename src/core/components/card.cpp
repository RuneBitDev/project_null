#include "core/components/card.h"


card::card(std::string c_card_id, std::string c_name, faction f_id, card_type c_type,
           std::string c_rarity, bool c_is_unlocked)
    : card_id(std::move(c_card_id)), name(std::move(c_name)), faction_id(f_id), c_type(c_type),
    rarity(std::move(c_rarity)), is_unlocked(c_is_unlocked) {}


std::unique_ptr<card> card::clone() const {
    return std::make_unique<card>(*this);
}

void card::add_ability(std::shared_ptr<ability> ability_ptr) {
    card_abilities.push_back(std::move(ability_ptr));
}

