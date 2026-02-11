#include "core/components/ability/ability_strike.h"
#include "core/components/card_unit.h"
#include "core/components/board.h"

ability_strike::ability_strike(std::string id, std::string name, const std::string &type, std::vector<ParamValue> params)
    : ability(id, name, type, params) {
    if (params.size() >= 2) {
        damage_amount = std::get<int>(params[0]);

        std::string type_str = std::get<std::string>(params[1]);
        if (type_str == "LETHAL") s_type = strike_type::LETHAL;
        else if (type_str == "BREAKER") s_type = strike_type::BREAKER;
        else if (type_str == "SPLASH") s_type = strike_type::SPLASH;
    }
}

void ability_strike::execute(ability_context &ctx) {
    if (s_type == strike_type::LETHAL) {
        execute_lethal(ctx);
    }
}

void ability_strike::execute_lethal(ability_context &ctx) {
    std::vector<card_location> targets = ctx.game_board.get_max_value_locations_on_board(value_type::STRENGTH);

    for (const auto& loc : targets) {
        const auto& row = ctx.game_board.get_row_cards(static_cast<int>(loc.side), static_cast<int>(loc.type));
        
        if (loc.index < row.size()) {
            if (auto* unit = dynamic_cast<card_unit*>(row[loc.index].get())) {
                unit->set_strength(0); // mark of death
            }
        }
    }
}