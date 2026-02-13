#ifndef PROJECT_NULL_PLAYER_H
#define PROJECT_NULL_PLAYER_H
#include <memory>
#include <vector>
#include "deck.h"
#include "card.h"
#include "../types.h"
#include "core/game_config.h"

class board;
class combat_manager;


class player {
public:
    player(std::string c_name, deck c_deck);

    // Move Card (std::move)
    void draw_card(int times); // from deck to hand
    void play_card(int index, board& b, row_side side, player& opponent, combat_manager& cm); // from hand to board
    void execute_play_card(std::unique_ptr<card> card_ptr, board &b, row_side side, player& opponent, combat_manager& cm); // from deck or hand to board
    void add_to_graveyard(std::unique_ptr<card> card_ptr); // from board to graveyard

    std::unique_ptr<card> pull_from_hand_by_id(const std::string& id); // mainly for summon ability

    // Getter & Setter
    deck& get_deck() { return player_deck; }
    const std::vector<std::unique_ptr<card>>& get_hand() const { return hand; }
    const std::vector<std::unique_ptr<card>>& get_graveyard() const { return graveyard; }
    bool get_has_passed() const { return has_passed; };
    bool get_has_played() const { return has_played; };
    void set_has_passed(bool n_has_passed) {has_passed = n_has_passed;};
    void set_has_played(bool n_has_played){ has_played = n_has_played;};
    row_side get_side() const { return assigned_side; }
    void set_side(row_side s) { assigned_side = s; }
    int get_lives() const { return lives; }
    void lose_live() { lives --; }


private:
    std::string name;
    int lives = game_config::player_config::LIVES;
    bool has_passed = false;
    bool has_played = false;
    deck player_deck;
    std::vector<std::unique_ptr<card>> hand;
    std::vector<std::unique_ptr<card>> graveyard;
    row_side assigned_side;

};

#endif //PROJECT_NULL_PLAYER_H