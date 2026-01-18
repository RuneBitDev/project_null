#ifndef PROJECT_NULL_DECK_H
#define PROJECT_NULL_DECK_H
#include <vector>
#include <memory>
#include "card.h"
#include "card_unit.h"

class deck {
    private:
    std::unique_ptr<card> leader;
    std::vector<std::unique_ptr<card>> cards;


    public:
    deck(std::unique_ptr<card> c_leader, std::vector<std::unique_ptr<card>> c_cards);

    std::unique_ptr<card> get_leader_card() const;
    std::unique_ptr<card> draw_top_card();
    int get_size() const;

    bool is_valid();
    void shuffle();
};

#endif //PROJECT_NULL_DECK_H