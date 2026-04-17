#ifndef PROJECT_NULL_GAME_STATE_H
#define PROJECT_NULL_GAME_STATE_H
#include "state.h"
#include "core/factory.h"
#include "core/match_manager.h"
#include "visual/texture_factory.h"
#include "visual/managers/widget_manager_match.h"


class match_state : public state {
public:
    match_state(player player1, player player2, factory& factory, texture_factory& texture_factory);
    ~match_state();
    void handle_input(state_manager& manager) override;
    void update(float dt, renderer& renderer) override;
    void render(renderer& ren) override;

    std::string get_name() const override { return "game_state"; }

private:
    factory& data_factory;
    texture_factory& tex_factory;

    std::unique_ptr<match_manager> match; // logic


    bool game_over = false;
    float end_screen_timer = 10.0f;

    // initialization hand drawing
    const int INITIAL_HAND_SIZE = game_config::player_config::HAND_SIZE;
    const float TIME_BETWEEN_CARDS = 0.1f;
    int cards_drawn = 0;
    float draw_timer = 0.0f;
    bool intro_delay_finished = false;
    float intro_timer = 3.0f;
};

#endif //PROJECT_NULL_GAME_STATE_H