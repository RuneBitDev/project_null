#ifndef PROJECT_NULL_MATCH_MANAGER_H
#define PROJECT_NULL_MATCH_MANAGER_H
#include "components/board.h"
#include "components/player.h"
#include "types.h"
#include "combat_manager.h"
#include <map>

enum class round_state { WIN, LOSE, DRAW };
enum class game_end { WIN, LOSE, DRAW, CONTINUE };

class match_manager {
public:
    match_manager(player p1, player p2);

    // main engine hooks
    void update(float dt);
    void handle_input();

    // gameplay actions
    void play_card_from_hand(int hand_index, row_side side);
    void pass_turn(row_side side);

    // scoring and state queries
    int get_player_score(row_side side) const;
    const board& get_board() const { return game_board; }
    const player& get_player(row_side side) const;
    row_side get_active_side() const { return active_player_side; }

private:
    combat_manager combat;
    board game_board;
    player p1;
    player p2;

    row_side active_player_side;
    std::map<row_side, std::vector<int>> round_scores;

    int current_round = 1;
    float ai_timer = 0.0f;

    // internal logic helpers
    void execute_ai_turn();
    game_end end_round();
    void switch_turn();
};

#endif //PROJECT_NULL_MATCH_MANAGER_H