#include "game/components/ability/ability_weather.h"


ability_weather::ability_weather(std::string id, std::string name, std::string type, std::vector<ParamValue> params)
    : ability(std::move(id), std::move(name), std::move(type), std::move(params)) {

    for (const auto& p : ability_params) {
        if (std::holds_alternative<std::string>(p)) {
            if (std::get<std::string>(p) == "MELEE") {
                target_row_type = row_type::MELEE;
            }  else if (std::get<std::string>(p) == "RANGED") {
                target_row_type = row_type::RANGED;
            } else if (std::get<std::string>(p) == "HEAVY") {
                target_row_type = row_type::HEAVY;
            } else if (std::get<std::string>(p) == "NET") {
                target_row_type = row_type::NET;
            } else if (std::get<std::string>(p) == "CLEAR") {
                clear_weather = true;
            }
        }
    }
}

void ability_weather::execute(ability_context &ctx) {
    if (clear_weather) {
        // Unapply weather for the whole board
        ctx.game_board.for_each_card([](card& c) {
            if (auto* unit = dynamic_cast<card_unit*>(&c)) {
                unit->set_weathered(false);
            }
        });
        ctx.game_board.set_row_weather(row_type::MELEE, false);
        ctx.game_board.set_row_weather(row_type::RANGED, false);
        ctx.game_board.set_row_weather(row_type::HEAVY, false);
        ctx.game_board.set_row_weather(row_type::NET, false);
    } else {
        // Apply weather to a specific row
        ctx.game_board.set_row_weather(target_row_type, true);
        for (int side = 0; side < 2; ++side) {
            const auto& row_cards = ctx.game_board.get_row_cards(side, static_cast<int>(target_row_type));
            for (const auto& card_ptr : row_cards) {
                if (auto* unit = dynamic_cast<card_unit*>(card_ptr.get())) {
                    unit->set_weathered(true);
                }
            }
        }
    }
}