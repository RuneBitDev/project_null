#ifndef PROJECT_NULL_GAME_STATE_H
#define PROJECT_NULL_GAME_STATE_H
#include "state.h"
#include "core/components/board.h"
#include "core/components/player.h"

enum class round_state {WIN, LOSE, DRAW};
enum class game_end {WIN, LOSE, DRAW, CONTINUE};

class game_state : public state {

public:
    game_state(player player1, player player2);

    void handle_input(state_manager& manager) override;
    void update(float dt) override;
    void render(renderer& ren) override;

    std::string get_name() const override { return "game_state"; };


private:
    board game_board;
    player p1;
    player p2;

    int active_player = 1;
    std::map<row_side, std::vector<int>> round_scores;
    round_state end_of_round_state;
    int current_round = 1;
    float ai_timer = 0.0f;

    int p1_cards_played = 0;
    int p2_cards_played = 0;

    void execute_ai_turn();
    game_end end_round();
    int get_clicked_hand_card_index() const;
    void handle_board_interactions() const;
};

#endif //PROJECT_NULL_GAME_STATE_H