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
    void draw_card(int times);
    void play_card(int index, board& b, row_side side);
    std::unique_ptr<card> pull_from_hand_by_id(const std::string& id);

    deck& get_deck() { return player_deck; }
    const std::vector<std::unique_ptr<card>>& get_hand() const { return hand; }
    const std::vector<std::unique_ptr<card>>& get_graveyard() const { return graveyard; }

    bool get_has_passed() const { return has_passed; };
    bool get_has_played() const { return has_played; };
    void set_has_passed(bool n_has_passed) {has_passed = n_has_passed;};
    void set_has_played(bool n_has_played){ has_played = n_has_played;};

private:
    std::string name;
    bool has_passed = false;
    bool has_played = false;
    deck player_deck;
    std::vector<std::unique_ptr<card>> hand;
    std::vector<std::unique_ptr<card>> graveyard;

};

#endif //PROJECT_NULL_PLAYER_H