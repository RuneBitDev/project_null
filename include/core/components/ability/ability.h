#ifndef PROJECT_NULL_ABILITY_H
#define PROJECT_NULL_ABILITY_H
#include <utility>
#include <variant>
#include "core/components/board.h"
#include "core/components/player.h"

class combat_manager;

using ParamValue = std::variant<int, std::string>;

struct ability_context {
    combat_manager& manager;
    player& owner;
    player& opponent;
    card_unit* caster;
};

class ability {
public:
    ability(std::string id, std::string name, std::string type, std::vector<ParamValue> params)
        : ability_id(std::move(id)), ability_name(std::move(name)),
          ability_type(std::move(type)), ability_params(std::move(params)) {}

    virtual ~ability() = default;
    virtual void execute(ability_context& ctx) = 0;

    const std::string& get_type() const { return ability_type; }
    const std::string& get_id() const { return ability_id; }

protected:
    std::string ability_id;
    std::string ability_name;
    std::string ability_type;
    std::vector<ParamValue> ability_params;
};

#endif //PROJECT_NULL_ABILITY_H