#include "game/components/ability/ability_modifier.h"
#include <iostream>


ability_modifier::ability_modifier(std::string id, std::string name, const std::string &type, std::vector<ParamValue> params)
    : ability(std::move(id), std::move(name), type, std::move(params)) {

    // 1. Assign modifier type based on ability type first
    if (type == "WEATHER") {
        m_type = modifier_type::SET;
    } else if (type == "BUFF") {
        m_type = modifier_type::ADD;
    }

    // 2. Parse parameters
    for (const auto& p : ability_params) {
        if (std::holds_alternative<std::string>(p)) {
            std::string val = std::get<std::string>(p);
            if (val == "MELEE") target_row = row_type::MELEE;
            else if (val == "RANGED") target_row = row_type::RANGED;
            else if (val == "HEAVY") target_row = row_type::HEAVY;
            else if (val == "NET") target_row = row_type::NET;
            else if (val == "CLEAR") clear_weather = true;
        }
        if (std::holds_alternative<int>(p)) {
            status_amount = std::get<int>(p);
        }
    }
}

void ability_modifier::execute(ability_context &ctx) {
    if (m_type == modifier_type::SET) {
        execute_weather(ctx);
    } else if (m_type == modifier_type::ADD) {
        execute_buff(ctx);
    }
}

void ability_modifier::execute_buff(const ability_context &ctx) const {
    std::cout << "INSIDE EXECUTE BUFF" << std::endl;
    ctx.game_board.save_modifiers(ctx.owner.get_side(), target_row, m_type, status_amount);
}


void ability_modifier::execute_weather(const ability_context& ctx) const {
    std::cout << "INSIDE EXECUTE WEATHER" << std::endl;
    if (clear_weather) {
        // To clear weather, we only need to clear the board's modifier map.
        // You should add a clear_all_modifiers() or similar method to board.cpp
        //ctx.game_board.clear_all_modifiers(); // This is much cleaner than iterating cards
    } else {
        // Apply weather to the same row for both players on the board
        ctx.game_board.save_modifiers(row_side::PLAYER, target_row, m_type, status_amount);
        ctx.game_board.save_modifiers(row_side::OPPONENT, target_row, m_type, status_amount);
    }
}
