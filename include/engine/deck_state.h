#ifndef PROJECT_NULL_DECK_STATE_H
#define PROJECT_NULL_DECK_STATE_H
#include "game/engine/state.h"

class deck_state : public state {
public:
    deck_state();

    void handle_input(state_manager& manager) override;
    void update(float dt) override;
    void render(renderer& ren) override;

private:

};

#endif //PROJECT_NULL_DECK_STATE_H