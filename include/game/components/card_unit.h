#ifndef PROJECT_NULL_CARD_UNIT_H
#define PROJECT_NULL_CARD_UNIT_H
#include "card.h"
#include <string>
#include <memory>
#include <iostream>

class card_unit : public card {
public:
    card_unit(std::string card_id, std::string name, std::string faction_id, std::string card_type,
        std::string rarity, int slots, bool is_unlocked, int strength, std::string range_type);

    std::unique_ptr<card> clone() const override;

    int get_strength() const override;
    void set_weathered(bool state, int value);
    std::string get_range_type() const override;
    void set_strength(int new_strength);
    void set_range_type(std::string new_range_type);

private:
    int strength;
    std::string range_type;
    bool weathered = false;
    int modifier = 0;

};

#endif //PROJECT_NULL_CARD_UNIT_H