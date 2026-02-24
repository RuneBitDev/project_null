#ifndef PROJECT_NULL_GAME_STATE_H
#define PROJECT_NULL_GAME_STATE_H
#include "state.h"
#include "core/match_manager.h"


class game_state : public state {
public:
    game_state(player player1, player player2);
    ~game_state();
    void handle_input(state_manager& manager) override;
    void update(float dt, renderer& renderer) override;
    void render(renderer& ren) override;

    std::string get_name() const override { return "game_state"; }

private:
    std::unique_ptr<match_manager> match;
    Texture2D background;
};

#endif //PROJECT_NULL_GAME_STATE_H