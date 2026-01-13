#ifndef PROJECT_NULL_CARD_UNIT_H
#define PROJECT_NULL_CARD_UNIT_H
#include "card.h"
#include <string>

class card_unit : public card {
    private:
    int strength;
    bool is_hero;

    public:
    card_unit(int id, std::string name, std::string faction, int strength, bool is_hero);
    void display() const override;
    int get_strength() const;
    void set_strength(int strength);
    bool get_is_hero() const;

};

#endif //PROJECT_NULL_CARD_UNIT_H