#ifndef PROJECT_NULL_MATCH_MANAGER_H
#define PROJECT_NULL_MATCH_MANAGER_H

#include "components/board.h"
#include "components/player.h"
#include "types.h"

class match_manager {
public:
    match_manager(player p1, player p2);

    // turn logic
    void update(float dt);
    void switch_turn();
    void pass_turn(row_side side);

    // actions
    void play_card_from_hand(int hand_index, row_side, row_type);

    // scoring
    int get_player_score(row_side side) const;
    void update_round_status();

    // getters for renderer
    const board& get_board() const { return game_board; }
    const player& get_player(row_side side) const;

private:
    board game_board;
    player p1;
    player p2;

    row_side active_player_side;
    int current_round = 1;
};

#endif //PROJECT_NULL_MATCH_MANAGER_H