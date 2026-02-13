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
        if (target_str == "MAX") strike_target = strike_target::MAX;
        else if (target_str == "MIN") strike_target = strike_target::MIN;

        std::string target_type_str = std::get<std::string>(params[2]);
        if (target_type_str == "STRENGTH") s_target_type = value_type::STRENGTH;
        else if (target_type_str == "ARMOR") s_target_type = value_type::ARMOR;
        else if (target_type_str == "ATTACK") s_target_type = value_type::ATTACK;

        damage_amount = std::get<int>(params[3]);
        std::cout << "[STRIKE ABILITY]: Values: " << type_str << ", " << damage_amount << std::endl;
    }
}

void ability_strike::execute(ability_context &ctx) {
    switch (s_type) {
        case strike_type::LETHAL:   execute_lethal(ctx);    break;
        case strike_type::SPLASH:   execute_splash(ctx);    break;
        case strike_type::FRAG:     execute_frag(ctx);      break;
        case strike_type::BARRAGE:  execute_barrage(ctx);   break;
        case strike_type::STATUS:   execute_status(ctx);    break;
        case strike_type::BREAKER:  execute_breaker(ctx);   break;
    }
}

void ability_strike::execute_lethal(ability_context &ctx) {
    std::vector<card_location> targets;
    board& b = ctx.manager.get_board();

    switch (strike_target) {
        case strike_target::MAX:
            targets = b.get_max_value_locations_on_board(s_target_type);
            break;
        case strike_target::MIN:
            // targets = b.get_min_value_locations_on_board(s_target_type);
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
    board& b = ctx.manager.get_board();
    row_side side = ctx.opponent.get_side();
    std::vector<card_location> centers = b.get_max_value_locations_on_side(s_target_type, side);

    if (centers.empty()) return;

    card_location center_loc = centers[0];
    ctx.manager.apply_damage_by_value(damage_amount, center_loc);;

    int side_dmg = (damage_amount + 1) / 2;
    auto& center_row = b.get_row_cards(static_cast<int>(center_loc.side), static_cast<int>(center_loc.type));

    // left neighbor
    if (center_loc.index > 0) {
        card_location left_loc = center_loc;
        left_loc.index--;
        ctx.manager.apply_damage_by_value(side_dmg, left_loc);
    }

    // right neighbor
    if (center_loc.index < (int)center_row.size() - 1) {
        card_location right_loc = center_loc;
        right_loc.index++;
        ctx.manager.apply_damage_by_value(side_dmg, right_loc);
    }
}

void ability_strike::execute_frag(ability_context &ctx) {
    board& b = ctx.manager.get_board();
    row_side side = ctx.opponent.get_side();

    std::vector<card_location> centers = b.get_max_value_locations_on_side(s_target_type, side);
    if (centers.empty()) return;

    card_location center_loc = centers[0];
    int side_dmg = (damage_amount + 1) / 2;

    ctx.manager.apply_damage_by_value(damage_amount, center_loc);

    // horizontal splash
    card_location left = center_loc; left.index--;
    ctx.manager.apply_damage_by_value(side_dmg, left);

    card_location right = center_loc; right.index++;
    ctx.manager.apply_damage_by_value(side_dmg, right);

    // vertical splash
    if (center_loc.type != row_type::MELEE) {
        card_location front = center_loc;
        front.type = static_cast<row_type>(static_cast<int>(center_loc.type) - 1);
        ctx.manager.apply_damage_by_value(side_dmg, front);
    }
    if (center_loc.type != row_type::NET) {
        card_location back = center_loc;
        back.type = static_cast<row_type>(static_cast<int>(center_loc.type) + 1);
        ctx.manager.apply_damage_by_value(side_dmg, back);
    }
}