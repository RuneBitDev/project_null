#include "core/components/ability/ability_strike.h"
#include <iostream>
#include "core/combat_manager.h"
#include "core/components/card_unit.h"
#include "core/components/board.h"

ability_strike::ability_strike(std::string id, std::string name, const std::string &type, std::vector<ParamValue> params)
    : ability(id, name, type, params) {
    if (params.size() >= 2) {
        std::string type_str = std::get<std::string>(params[0]);
        if (type_str == "LETHAL") s_type = strike_type::LETHAL;
        else if (type_str == "BREAKER") s_type = strike_type::BREAKER;
        else if (type_str == "SPLASH") s_type = strike_type::SPLASH;
        else if (type_str == "FRAG") s_type = strike_type::FRAG;
        else if (type_str == "BARRAGE") s_type = strike_type::BARRAGE;
        else if (type_str == "STATUS") s_type = strike_type::STATUS;
        std::string target_str = std::get<std::string>(params[1]);
        if (target_str == "STRENGTH") s_target = strike_target::MAX_STRENGTH;
        else if (target_str == "ARMOR") s_target = strike_target::MAX_ARMOR;
        else if (target_str == "ATTACK") s_target = strike_target::MAX_ATTACK;
        damage_amount = std::get<int>(params[2]);
        std::cout << "[!] values: " << type_str << ", " <<damage_amount << std::endl;
    }
}

void ability_strike::execute(ability_context &ctx) {
    if (s_type == strike_type::LETHAL) {
        execute_lethal(ctx);
    }
}

void ability_strike::execute_lethal(ability_context &ctx) {
    std::vector<card_location> targets;
    board& b = ctx.manager.get_board();

    switch (s_target) {
        case strike_target::MAX_STRENGTH:
            targets = b.get_max_value_locations_on_board(value_type::STRENGTH);
            break; // Essential!
        case strike_target::MAX_ARMOR:
            targets = b.get_max_value_locations_on_board(value_type::ARMOR);
            break;
        case strike_target::MAX_ATTACK:
            targets = b.get_max_value_locations_on_board(value_type::ATTACK);
            break;
        default:
            std::cout << "[!] Lethal strike has no valid target type." << std::endl;
            return;
    }

    for (const auto& loc : targets) {
        auto& row = b.get_row_cards(static_cast<int>(loc.side), static_cast<int>(loc.type));

        if (loc.index >= 0 && loc.index < (int)row.size()) {
            ctx.manager.apply_damage_by_value(damage_amount, loc);
        }
    }
}


void ability_strike::execute_splash(ability_context &ctx) {
    // std::vector<card_location> targets = ctx.game_board.get_row_cards();
}