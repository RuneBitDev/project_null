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
    ability(std::string id, std::string name, std::string type, std::vector<ParamValue> params);

    virtual ~ability() = default;
    virtual void execute(ability_context& ctx) = 0;

    const std::string& get_type() const { return ability_type; }
    const std::string& get_id() const { return ability_id; }

protected:
    std::string ability_id;
    std::string ability_name;
    std::string ability_type;
    std::vector<ParamValue> ability_params;

    std::string get_param_string(size_t index, const std::string& default_val = "") const;
    int get_param_int(size_t index, int default_val = 0) const;

};

#endif //PROJECT_NULL_ABILITY_H