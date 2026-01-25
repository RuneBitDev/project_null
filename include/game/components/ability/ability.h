#ifndef PROJECT_NULL_ABILITY_H
#define PROJECT_NULL_ABILITY_H
#include <utility>
#include "game/components/board.h"
#include "game/components/player.h"

struct ability_context {
    board& game_board;
    player& owner;
    player& opponent;

};

class ability {
public:
    ability(std::string id, std::string name, std::string type, std::vector<std::string> params)
        : ability_id(std::move(id)), ability_name(std::move(name)),
          ability_type(std::move(type)), ability_params(std::move(params)) {}

    virtual ~ability() = default;
    virtual void execute(ability_context& ctx) = 0;
    const std::string& get_id() const { return ability_id; }

private:
    std::string ability_id;
    std::string ability_name;
    std::string ability_type;
    std::vector<std::string> ability_params;
};

#endif //PROJECT_NULL_ABILITY_H