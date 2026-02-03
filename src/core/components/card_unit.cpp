#include "game/components/card.h"
#include "game/components/card_unit.h"
#include "game/components/board.h"
#include <utility>
#include <algorithm>

card_unit::card_unit(std::string card_id, std::string name, std::string faction_id, std::string card_type,
        std::string rarity, bool is_unlocked, int strength, std::string range_type, int armor, int attack)
    : card(std::move(card_id), std::move(name), std::move(faction_id), std::move(card_type), std::move(rarity), is_unlocked),
        strength(strength), range_type(std::move(range_type)), armor(armor), attack(attack) {}


std::unique_ptr<card> card_unit::clone() const {
    return std::make_unique<card_unit>(*this);
}

int card_unit::get_strength() const {
    int val = strength;
    for (auto& modifier : modifiers) {
        val = apply_mod_math(val, modifier);
    }
    return val;
}

int card_unit::get_virtual_strength(const board &b, row_side side, row_type type) const {
    int val = get_strength();

    const auto& board_mods = b.get_modifiers(side, type);
    for (const auto& b_modifier : board_mods) {
        val = apply_mod_math(val, b_modifier);
    }

    return val;

}

void card_unit::set_modifier(bool state, int value) {
    modified = state;
    modifier_value = value;
}

void card_unit::save_modifier(modifier_type m_type, int m_value) {
    modifiers.emplace_back(m_type, m_value);
    // Sort that shit
    auto get_priority = [](modifier_type t) {
        switch (t) {
            case modifier_type::SET:        return 0;
            case modifier_type::ADD:        return 1;
            case modifier_type::SUBTRACT:   return 2;
            case modifier_type::MULTIPLY:   return 3;
            default:                        return 4;
        }
    };

    std::ranges::sort(modifiers, [&](const auto& a, const auto& b) {
        return get_priority(std::get<0>(a)) < get_priority(std::get<0>(b));
    });
}

void card_unit::delete_modifier(modifier_type m_type) {
    auto it = modifiers.begin();
    while (it != modifiers.end()) {
        if (std::get<0>(*it) == m_type) {
            it = modifiers.erase(it); // Returns the next valid iterator
        } else {
            ++it;
        }
    }
}

// HELPER FUNCTIONS

int card_unit::apply_mod_math(int base_value, const std::tuple<modifier_type, int> &modifier) const{
    modifier_type m_type = std::get<0>(modifier);
    int m_value = std::get<1>(modifier);

    switch (m_type) {
        case modifier_type::SET:        return m_value;
        case modifier_type::ADD:        return base_value + m_value;
        case modifier_type::SUBTRACT:   return base_value - m_value;
        case modifier_type::MULTIPLY:   return base_value * m_value;
        default:                        return base_value;
    }
}
