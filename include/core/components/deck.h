#ifndef PROJECT_NULL_DECK_H
#define PROJECT_NULL_DECK_H
#include <vector>
#include <memory>
#include "card.h"

class deck {
public:
    deck(std::unique_ptr<card> c_leader, std::vector<std::unique_ptr<card>> c_cards);

    std::unique_ptr<card> get_leader_card() const;
    std::vector<card*> get_card_ptrs() const;
    std::unique_ptr<card> draw_top_card();
    std::unique_ptr<card> pull_card_by_id(const std::string& id);
    int get_size() const;

    bool is_valid();
    void shuffle();
    bool is_empty() const;
    size_t get_count() const;

private:
    std::unique_ptr<card> leader;
    std::vector<std::unique_ptr<card>> cards;

};

#endif //PROJECT_NULL_DECK_H