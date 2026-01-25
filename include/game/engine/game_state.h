#ifndef PROJECT_NULL_GAME_STATE_H
#define PROJECT_NULL_GAME_STATE_H
#include "state.h"
#include "../components/board.h"
#include "../components/player.h"


class game_state : public state {

public:
    game_state(player player1, player player2);

    void handle_input(state_manager& manager) override;
    void update(float dt) override;
    void render(renderer& ren) override;

    std::string get_name() const override { return "game_state"; };
    player& get_player(row_side side);
    player& get_opponent(row_side side);

private:
    board game_board;
    player p1;
    player p2;

    int active_player = 1;
    bool round_ended = false;
    int current_round = 0;
    float ai_timer = 0.0f;

    player& get_other_player();
    void execute_ai_turn();
};

#endif //PROJECT_NULL_GAME_STATE_H