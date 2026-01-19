#ifndef PROJECT_NULL_PLAYER_H
#define PROJECT_NULL_PLAYER_H
#include <memory>
#include <vector>
#include "deck.h"
#include "card.h"
#include "board.h"

class player {
public:
    player(std::string c_name, deck c_deck);
    void display_hand() const;
    void draw_card(int times);
    void play_card(int index, board& b);

    const std::vector<std::unique_ptr<card>>& get_hand() const { return hand; }
    const std::vector<std::unique_ptr<card>>& get_graveyard() const { return graveyard; }

private:
    std::string name;
    bool has_passed = false;
    deck player_deck;
    std::vector<std::unique_ptr<card>> hand;
    std::vector<std::unique_ptr<card>> graveyard;


};

#endif //PROJECT_NULL_PLAYER_H