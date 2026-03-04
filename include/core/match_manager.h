#ifndef PROJECT_NULL_MATCH_MANAGER_H
#define PROJECT_NULL_MATCH_MANAGER_H
#include "components/board.h"
#include "components/player.h"
#include "types.h"
#include "combat_manager.h"
#include <map>
#include <optional>

enum class round_status {WIN, LOSS, DRAW};
enum class game_status {WIN, LOSS, DRAW, CONTINUE};

struct match_status {
    round_status round_status;
    game_status game_status;
};

enum class current_state {
    PLAYER_TURN,
    AI_TURN,
    FIREFIGHT,
    ROUND_OVER,
    GAME_OVER
};


class match_manager {
public:
    match_manager(player p1, player p2);

    // main engine hooks
    std::optional<match_status> update(float dt);
    void handle_input();

    // gameplay actions
    void play_card_from_hand(int hand_index, row_side side);
    void pass_turn(row_side side);

    // scoring and state queries
    int get_player_score(row_side side) const;
    const board& get_board() const { return game_board; }
    player& get_player(row_side side);
    const player& get_player(row_side side) const;
    row_side get_active_side() const { return active_player_side; }
    current_state get_current_state() const { return current_match_state; }

private:
    combat_manager combat;
    board game_board;
    player p1;
    player p2;

    current_state current_match_state = current_state::PLAYER_TURN;
    row_side active_player_side;
    int total_turns_this_round;
    int clash_counter;

    std::map<row_side, std::vector<int>> round_scores;

    std::optional<row_side> first_passer;
    int current_round = 1;
    float ai_timer = 0.0f;

    // internal logic helpers
    void prepare_next_round();
    round_status end_round();
    game_status end_game();
    void execute_ai_turn();
    void switch_turn();
};

#endif //PROJECT_NULL_MATCH_MANAGER_H