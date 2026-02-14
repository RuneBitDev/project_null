#include "core/components/ability/ability_modifier.h"
#include <iostream>

#include "core/combat_manager.h"


ability_modifier::ability_modifier(std::string id, std::string name, const std::string &type, std::vector<ParamValue> params)
    : ability(std::move(id), std::move(name), type, std::move(params)) {

    m_type     = string_to_modifier_type(get_param_string(0));
    target_row = string_to_row_type(get_param_string(1));
    m_target   = string_to_modifier_target(get_param_string(2));
    m_value    = get_param_int(3);

    std::cout << "[ABILITY MODIFIER] " << get_param_string(0) << " (" << m_value << ") on " << get_param_string(1) << std::endl;
}

void ability_modifier::execute(ability_context &ctx) {
    if (m_type == modifier_type::SET || m_type == modifier_type::CLEAR) {
        execute_weather(ctx);
    } else if (m_type == modifier_type::ADD || m_type == modifier_type::SUBTRACT || m_type == modifier_type::MULTIPLY) {
        execute_mod(ctx);
    }
}

void ability_modifier::execute_mod(const ability_context &ctx) const {
    std::cout << "INSIDE EXECUTE BUFF" << std::endl;
    ctx.manager.get_board().save_modifiers(ctx.owner.get_side(), target_row, m_type, m_value);
}


void ability_modifier::execute_weather(const ability_context& ctx) const {
    std::cout << "INSIDE EXECUTE WEATHER" << std::endl;
    if (m_type == modifier_type::CLEAR) {
        // clear all weather
        ctx.manager.get_board().clear_modifier(modifier_type::SET);
    } else {
        // Apply weather to the same row for both players on the board
        ctx.manager.get_board().save_modifiers(row_side::PLAYER, target_row, m_type, m_value);
        ctx.manager.get_board().save_modifiers(row_side::OPPONENT, target_row, m_type, m_value);
    }
}
