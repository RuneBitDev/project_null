#ifndef PROJECT_NULL_PLAYER_H
#define PROJECT_NULL_PLAYER_H
#include <memory>
#include <vector>
#include "deck.h"
#include "card.h"
#include "board.h"

class player {
    private:
    std::string name;
    bool has_passed = false;
    deck player_deck;
    std::vector<std::unique_ptr<card>> hand;
    std::vector<std::unique_ptr<card>> graveyard;

    public:
    player(std::string c_name, deck c_deck);
    void display_hand() const;
    void draw_card(int times);
    void play_card(int index, board& b);
};

#endif //PROJECT_NULL_PLAYER_H