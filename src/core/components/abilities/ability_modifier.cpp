#include "game/components/ability/ability_modifier.h"
#include <iostream>


ability_modifier::ability_modifier(std::string id, std::string name, std::string type, std::vector<ParamValue> params)
    : ability(std::move(id), std::move(name), type, std::move(params)) {

    for (const auto& p : ability_params) {
        if (std::holds_alternative<std::string>(p)) {
            if (std::get<std::string>(p) == "MELEE") {
                target_row = row_type::MELEE;
            }  else if (std::get<std::string>(p) == "RANGED") {
                target_row = row_type::RANGED;
            } else if (std::get<std::string>(p) == "HEAVY") {
                target_row = row_type::HEAVY;
            } else if (std::get<std::string>(p) == "NET") {
                target_row = row_type::NET;
            } else if (std::get<std::string>(p) == "CLEAR") {
                clear_weather = true;
            }
        }
        if (std::holds_alternative<int>(p)) {
            status_amount = std::get<int>(p);
        }

        if (type == "WEATHER") {
            modifier_type = modifier_type::SET;
        } else if (type == "BUFF") {
            modifier_type = modifier_type::ADD;
        }

    }
}

void ability_modifier::execute(ability_context &ctx) {
    if (modifier_type == modifier_type::SET) {
        execute_weather(ctx);
    } else if (modifier_type == modifier_type::ADD) {
        execute_buff(ctx);
    }
}

void ability_modifier::execute_buff(ability_context &ctx) {
    std::cout << "INSIDE EXECUTE BUFF" << std::endl;
    ctx.game_board.set_row_modifier(target_row, true); // this is a problem, it targets both sides
    const auto& row_cards = ctx.game_board.get_row_cards(0, static_cast<int>(target_row)); // side hardcoded for now
    for (const auto& card_ptr : row_cards) {
        if (auto* unit = dynamic_cast<card_unit*>(card_ptr.get())) {
            unit->set_modifier(true, status_amount);
        }
    }

}

void ability_modifier::execute_weather(ability_context& ctx) {
    std::cout << "INSIDE EXECUTE WEATHER" << std::endl;
    if (clear_weather) {
        // Unapply weather for the whole board
        ctx.game_board.for_each_card([](card& c) {
            if (auto* unit = dynamic_cast<card_unit*>(&c)) {
                unit->set_modifier(false, 0);
            }
        });
        ctx.game_board.set_row_modifier(row_type::MELEE, false);
        ctx.game_board.set_row_modifier(row_type::RANGED, false);
        ctx.game_board.set_row_modifier(row_type::HEAVY, false);
        ctx.game_board.set_row_modifier(row_type::NET, false);
    } else {
        // Apply weather to a specific row
        ctx.game_board.set_row_modifier(target_row, true);
        for (int side = 0; side < 2; ++side) {
            const auto& row_cards = ctx.game_board.get_row_cards(side, static_cast<int>(target_row));
            for (const auto& card_ptr : row_cards) {
                if (auto* unit = dynamic_cast<card_unit*>(card_ptr.get())) {
                    unit->set_modifier(true, 1);
                }
            }
        }
    }
}