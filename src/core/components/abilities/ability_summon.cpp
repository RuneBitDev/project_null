#include "game/components/ability/ability_summon.h"
#include <iostream>

ability_summon::ability_summon(std::string id, std::string name, std::string type, std::vector<std::string> params)
    : ability(std::move(id), std::move(name), std::move(type), params),
      target_ids(params) {}

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
            ctx.owner.execute_play_card(std::move(summoned_card), ctx.game_board, row_side::PLAYER, ctx.opponent);
        } else {
            std::cout << "[DEBUG] FAILED to find target_id: '" << target_id << "' in Deck or Hand." << std::endl;
        }
    }
}