#include "core/components/ability/ability_modifier.h"
#include <iostream>

#include "core/combat_manager.h"
#include "core/game_log.h"


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
    // add nicely formatted log
    game_log::add_break();
    std::string rowName = ctx.manager.get_board().get_row_name(target_row);
    std::string sideName = (ctx.owner.get_side() == row_side::PLAYER) ? "Your" : "Enemy";
    std::string valStr = (m_type == modifier_type::MULTIPLY) ? "x" + std::to_string(m_value) :
                         (m_type == modifier_type::ADD ? "+" : "-") + std::to_string(m_value);
    game_log::add("[MODIFIER]: " + sideName + " " + rowName + " units " + valStr, SKYBLUE);

    std::cout << "INSIDE EXECUTE BUFF" << std::endl;
    ctx.manager.get_board().save_modifiers(ctx.owner.get_side(), target_row, m_type, m_value);
}


void ability_modifier::execute_weather(const ability_context& ctx) const {
    if (m_type == modifier_type::CLEAR) {
        game_log::add_break();
        game_log::add(">> [SYSTEM]: Atmospheric conditions cleared <<", SKYBLUE);
        ctx.manager.get_board().clear_modifier(modifier_type::SET);
    } else {
        std::string rowName = ctx.manager.get_board().get_row_name(target_row);

        game_log::add_break();
        game_log::add("[WEATHER]: Extreme conditions on " + rowName + " rows!", SKYBLUE);
        game_log::add("   All units set to " + std::to_string(m_value) + " power.", DARKGRAY);

        ctx.manager.get_board().save_modifiers(row_side::PLAYER, target_row, m_type, m_value);
        ctx.manager.get_board().save_modifiers(row_side::OPPONENT, target_row, m_type, m_value);
    }
}
