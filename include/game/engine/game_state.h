#ifndef PROJECT_NULL_GAME_STATE_H
#define PROJECT_NULL_GAME_STATE_H
#include "state.h"
#include "visual/renderer.h"
#include "game/board.h"
#include "game/player.h"



class game_state : public state {
    private:
    board game_board;
    player p1;
    player p2;

    int current_round = 0;

    public:
    game_state(player player1, player player2);

    void handle_input(state_manager& manager) override;
    void update(float dt) override;
    void render(renderer& ren) override;
    void debug_display();

    std::string get_name() const override;
};

#endif //PROJECT_NULL_GAME_STATE_H