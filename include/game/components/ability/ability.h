#ifndef PROJECT_NULL_ABILITY_H
#define PROJECT_NULL_ABILITY_H
#include "game/components/board.h"
#include "game/components/player.h"

struct ability_context {
    board& game_board;
    player& owner;
    player& opponent;

};

class ability {
public:
    virtual ~ability() = default;
    virtual void execute(ability_context& ctx) = 0;
};

#endif //PROJECT_NULL_ABILITY_H