#include "game/components/card.h"
#include "game/components/card_unit.h"
#include <utility>
#include <algorithm>

card_unit::card_unit(std::string card_id, std::string name, std::string faction_id, std::string card_type,
        std::string rarity, int slots, bool is_unlocked, int strength, std::string range_type)
    : card(std::move(card_id), std::move(name), std::move(faction_id), std::move(card_type), std::move(rarity), slots, is_unlocked),
        strength(strength), range_type(std::move(range_type)) {}


std::unique_ptr<card> card_unit::clone() const {
    return std::make_unique<card_unit>(*this);
}

int card_unit::get_strength() const {
    int virtual_strength = strength;

    for (auto& modifier : modifiers) {
        modifier_type m_type = std::get<0>(modifier);
        int m_value = std::get<1>(modifier);

        if (m_type == modifier_type::ADD) {
            virtual_strength += m_value;
        } else if (m_type == modifier_type::SUBTRACT) {
            virtual_strength -= m_value;
        } else if (m_type == modifier_type::MULTIPLY) {
            virtual_strength *= m_value;
        } else if (m_type == modifier_type::SET) {
            virtual_strength = m_value;
        }
    }

    return virtual_strength;
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
        }
    };

    std::sort(modifiers.begin(), modifiers.end(), [&](const auto& a, const auto& b) {
        return get_priority(std::get<0>(a)) < get_priority(std::get<0>(b));
    });
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
