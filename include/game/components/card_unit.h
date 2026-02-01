#ifndef PROJECT_NULL_CARD_UNIT_H
#define PROJECT_NULL_CARD_UNIT_H
#include <string>
#include <memory>
#include <tuple>
#include "card.h"
#include "game/game_config.h"


class card_unit : public card {
public:
    card_unit(std::string card_id, std::string name, std::string faction_id, std::string card_type,
        std::string rarity, int slots, bool is_unlocked, int strength, std::string range_type);

    std::unique_ptr<card> clone() const override;
    void set_modifier(bool state, int value); // should save modifiers into a vector probably
    void safe_modifier(modifier_type m_type, int m_value);

    // GETTER & SETTER
    int get_strength() const override;
    void set_strength(int new_strength);
    std::string get_range_type() const override;
    void set_range_type(std::string new_range_type);

private:
    int strength;
    std::string range_type;
    bool modified = false;
    std::vector<std::tuple<modifier_type, int>> modifiers;
    int modifier_value = 0;

};

#endif //PROJECT_NULL_CARD_UNIT_H