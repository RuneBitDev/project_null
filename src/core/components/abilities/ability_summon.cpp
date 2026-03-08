#include "core/components/ability/ability_summon.h"
#include <iostream>

#include "raylib.h"
#include "core/combat_manager.h"
#include "core/game_log.h"

ability_summon::ability_summon(std::string id, std::string name, std::string type, std::vector<ParamValue> params)
    : ability(std::move(id), std::move(name), std::move(type), std::move(params)) {

    s_type = string_to_summon_type(get_param_string(0));

    for (size_t i = 1; i < ability_params.size(); ++i) {
        std::string id_val = get_param_string(i);
        if (!id_val.empty()) {
            target_ids.push_back(id_val);
        }
    }

    std::cout << "[ABILITY SUMMON] Type: " << get_param_string(0) << " | Targets: " << target_ids.size() << std::endl;
}

void ability_summon::execute(ability_context &ctx) {
    switch (s_type) {
        case summon_type::SUMMON:   execute_summon(ctx); break;
        case summon_type::NECRO:    execute_necro(ctx); break;
    }
}

void ability_summon::execute_summon(ability_context &ctx) {
    game_log::add_break();
    game_log::add("[ABILITY]: SUMMON calling reinforcements...", SKYBLUE);

    for (const auto& target_id : target_ids) {
        std::string source = "Unknown";
        std::unique_ptr<card> summoned_card = ctx.owner.get_deck().pull_card_by_id(target_id);

        if (summoned_card) {
            source = "DECK";
        } else {
            summoned_card = ctx.owner.pull_from_hand_by_id(target_id);
            if (summoned_card) source = "HAND";
        }

        if (summoned_card) {
            std::string unitName = summoned_card->get_name();

            if (auto* unit = dynamic_cast<card_unit*>(summoned_card.get())) {
                unit->change_stance_by_value(ctx.caster->get_stance());
            }

            // Log the specific unit arrival
            game_log::add("   + " + unitName + " deployed from " + source, DARKGRAY);

            ctx.owner.execute_play_card(std::move(summoned_card), ctx.manager.get_board(),
                                        ctx.owner.get_side(), ctx.opponent, ctx.manager);
        } else {
            game_log::add("   - Failed to locate " + target_id, RED);
        }
    }
}

void ability_summon::execute_necro(const ability_context &ctx) const {

}
