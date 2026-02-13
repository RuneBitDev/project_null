#include "core/components/ability/ability_summon.h"
#include <iostream>

#include "core/combat_manager.h"

ability_summon::ability_summon(std::string id, std::string name, std::string type, std::vector<ParamValue> params)
    : ability(std::move(id), std::move(name), std::move(type), std::move(params)) {

    for (const auto& p : ability_params) {
        if (std::holds_alternative<std::string>(p)) {
            target_ids.push_back(std::get<std::string>(p));
        }
    }
}

void ability_summon::execute(ability_context &ctx) {
    std::cout << "[DEBUG] Executing SUMMON for ability: " << get_id() << std::endl;

    for (const auto& target_id : target_ids) {
        std::cout << "[DEBUG] Searching for target_id: '" << target_id << "'" << std::endl;

        std::unique_ptr<card> summoned_card = ctx.owner.get_deck().pull_card_by_id(target_id);
        if (summoned_card) {
            std::cout << "[DEBUG] Found '" << target_id << "' in DECK." << std::endl;
        } else {
            summoned_card = ctx.owner.pull_from_hand_by_id(target_id);
            if (summoned_card) {
                std::cout << "[DEBUG] Found '" << target_id << "' in HAND." << std::endl;
            }
        }

        if (summoned_card) {
            ctx.owner.execute_play_card(std::move(summoned_card), ctx.manager.get_board(), ctx.owner.get_side(), ctx.opponent, ctx.manager);
        } else {
            std::cout << "[DEBUG] FAILED to find target_id: '" << target_id << "' in Deck or Hand." << std::endl;
        }
    }
}