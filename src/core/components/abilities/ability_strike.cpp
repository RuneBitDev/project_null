#include "core/components/ability/ability_strike.h"
#include <iostream>
#include <utility>
#include "core/combat_manager.h"
#include "core/components/card_unit.h"
#include "core/components/board.h"

ability_strike::ability_strike(std::string id, std::string name, const std::string &type, const std::vector<ParamValue> &params)
    : ability(std::move(id), std::move(name), type, params) {
    std::string type_str   = get_param_string(0);
    std::string target_str = get_param_string(1);
    std::string value_str  = get_param_string(2);
    damage_amount          = get_param_int(3);

    s_type          = string_to_strike_type(type_str);
    s_strike_target = string_to_strike_target(target_str);
    s_target_type   = string_to_value_type(value_str);

    std::cout << "[ABILITY STRIKE] " << type_str << " (" << damage_amount << " dmg) - targeting " << value_str << std::endl;
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

void ability_strike::execute_lethal(const ability_context &ctx) const {
    std::vector<card_location> targets;
    board& b = ctx.manager.get_board();

    switch (s_strike_target) {
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
        auto& row = b.get_row_cards(loc.side, loc.type);

        if (loc.index >= 0 && loc.index < static_cast<int>(row.size())) {
            ctx.manager.apply_damage_by_value(damage_amount, loc);
        }
    }
}


void ability_strike::execute_splash(const ability_context &ctx) const {
    board& b = ctx.manager.get_board();
    row_side side = ctx.opponent.get_side();
    std::vector<card_location> centers = b.get_max_value_locations_on_side(s_target_type, side);

    if (centers.empty()) return;

    card_location center_loc = centers[0];
    ctx.manager.apply_damage_by_value(damage_amount, center_loc);;

    int side_dmg = (damage_amount + 1) / 2;
    auto& center_row = b.get_row_cards(center_loc.side, center_loc.type);

    // left neighbor
    if (center_loc.index > 0) {
        card_location left_loc = center_loc;
        left_loc.index--;
        ctx.manager.apply_damage_by_value(side_dmg, left_loc);
    }

    // right neighbor
    if (center_loc.index < static_cast<int>(center_row.size()) - 1) {
        card_location right_loc = center_loc;
        right_loc.index++;
        ctx.manager.apply_damage_by_value(side_dmg, right_loc);
    }
}

void ability_strike::execute_frag(const ability_context &ctx) const {
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

void ability_strike::execute_barrage(ability_context &ctx) {

}

void ability_strike::execute_breaker(ability_context &ctx) {

}

void ability_strike::execute_status(ability_context &ctx) {

}
