#ifndef PROJECT_NULL_DECK_H
#define PROJECT_NULL_DECK_H
#include <vector>
#include "card.h"
#include "card_unit.h"

class deck {
    private:
    card leader;
    std::vector<card> special_cards;
    std::vector<card_unit> unit_cards;

    public:
    deck(card leader, std::vector<card> special_cards, std::vector<card_unit> c_unit_cards);
    bool is_valid();

    card get_leader() const;
    const std::vector<card>& get_special() const;
    const std::vector<card_unit>& get_units() const;

};

#endif //PROJECT_NULL_DECK_H