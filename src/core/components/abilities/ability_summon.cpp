#include "game/components/ability/ability_summon.h"

ability_summon::ability_summon(std::vector<std::string> ids)
    : target_ids(std::move(ids)) {}

void ability_summon::execute(ability_context &ctx) {
    for (const std::string& target_id : target_ids) {
        std::unique_ptr<card> c = ctx.owner.get_deck().pull_card_by_id(target_id);
        if (!c) {
            c = ctx.owner.pull_from_hand_by_id(target_id);
        }

        if (c) {
            row_type type = (c->get_range_type() == "MELEE") ? row_type::MELEE : row_type::RANGED;
            ctx.game_board.add_card(std::move(c), row_side::PLAYER, type);
        }
    }
}
