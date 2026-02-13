#include "core/components/ability/ability_modifier.h"
#include <iostream>

#include "core/combat_manager.h"


ability_modifier::ability_modifier(std::string id, std::string name, const std::string &type, std::vector<ParamValue> params)
    : ability(std::move(id), std::move(name), type, std::move(params)) {

    // parse parameters
    for (const auto& p : ability_params) {
        if (const auto* str_ptr = std::get_if<std::string>(&p)) {
            const std::string& val = *str_ptr;

            if      (val == "SET")      m_type = modifier_type::SET;
            else if (val == "ADD")      m_type = modifier_type::ADD;
            else if (val == "SUB")      m_type = modifier_type::SUBTRACT;
            else if (val == "CLEAR")    m_type = modifier_type::CLEAR;

            else if (val == "MELEE")    target_row = row_type::MELEE;
            else if (val == "RANGED")   target_row = row_type::RANGED;
            else if (val == "HEAVY")    target_row = row_type::HEAVY;
            else if (val == "NET")      target_row = row_type::NET;

            else if (val == "ST")       m_target = modifier_target::STRENGTH;
            else if (val == "AR")       m_target = modifier_target::ARMOR;
            else if (val == "AT")       m_target = modifier_target::ATTACK;
        }
        if (std::holds_alternative<int>(p)) {
            m_value = std::get<int>(p);
        }
    }
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
