#include "core/components/ability/ability_spy.h"

#include "raylib.h"
#include "core/game_log.h"


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

    // log
    game_log::add_break();
    std::string sideName = (ctx.owner.get_side() == row_side::PLAYER) ? "Player" : "Opponent";
    game_log::add("[ABILITY]: SPY activated by " + sideName, SKYBLUE);
    std::string cardSuffix = (number_of_cards > 1) ? " cards." : " card.";
    game_log::add("    + Intelligence gathered: Drawn " + std::to_string(number_of_cards) + cardSuffix, GRAY);
}
