#include "core/components/ability/ability_spy.h"

#include <iostream>

ability_spy::ability_spy(std::string id, std::string name, std::string type, std::vector<ParamValue> params)
    : ability(std::move(id), std::move(name), std::move(type), std::move(params)) {

    for (const auto& p : ability_params) {
        if (std::holds_alternative<int>(p)) {
            number_of_cards = std::get<int>(p);
            break;
        }
    }
}

void ability_spy::execute(ability_context &ctx) {
    ctx.owner.draw_card(number_of_cards);
}
