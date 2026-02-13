#include "core/combat_manager.h"
#include <iostream>

combat_manager::combat_manager(board &b, player &p1, player &p2)
    : game_board(b), p1(p1), p2(p2) {}

void combat_manager::update(float dt) {
    if (!is_busy()) return;

    timer += dt;
    if (timer >= STEP_DELAY) {
        advance_phase();
        timer = 0.0f;
    }
}

void combat_manager::firefight() {
    current_phase = combat_phase::PLAYER_ATTACKING;
    timer = 0.0f;
}

void combat_manager::cleanup_dead_units() const {
    for (int s = 0; s < 2; ++s) {
        auto side = static_cast<row_side>(s);
        player& owner = (side == row_side::PLAYER) ? p1 : p2;

        for (int t = 0; t < 5; ++t) {
            auto& row = game_board.get_row_cards(s, t);

            auto it = row.begin();
            while (it != row.end()) {
                if ((*it)->is_dead()) {
                    std::cout << "[GRAVEYARD] " << (*it)->get_name() << "\n";
                    owner.add_to_graveyard(std::move(*it));
                    it = row.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }
}

void combat_manager::advance_phase() {
    switch (current_phase) {
        case combat_phase::PLAYER_ATTACKING:
            resolve_turn(p1);
            current_phase = combat_phase::OPPONENT_ATTACKING;
            break;
        case combat_phase::OPPONENT_ATTACKING:
            resolve_turn(p2);
            current_phase = combat_phase::CLEANUP;
            break;
        case combat_phase::CLEANUP:
            cleanup_dead_units();
            current_phase = combat_phase::IDLE;
            break;
        case combat_phase::IDLE:
            break;
    }
}

void combat_manager::resolve_turn(const player& p) const {
    row_side my_side = p.get_side();
    row_side enemy_side = (my_side == row_side::PLAYER) ? row_side::OPPONENT : row_side::PLAYER;

    std::cout << "\n--- RESOLVING TURN FOR " << (my_side == row_side::PLAYER ? "PLAYER" : "OPPONENT") << " ---" << std::endl;

    for (int r = 0; r < 4; r++) {
        auto current_row = static_cast<row_type>(r);
        auto& units = game_board.get_row_cards(static_cast<int>(my_side), r);

        for (size_t i = 0; i < units.size(); ++i) {
            auto* unit = dynamic_cast<card_unit*>(units[i].get());

            if (!unit) {
                std::cout << "[!] Card at row " << r << " index " << i << " is not a unit. Skipping." << std::endl;
                continue;
            }

            if (unit->get_stance() == stances::DEFENSIVE) {
                std::cout << "[IDLE] Unit '" << unit->get_name() << "' is Defensive. No attack." << std::endl;
                continue;
            }

            card_location target_loc = find_best_target(enemy_side, current_row, unit->get_stance());

            if (target_loc.index != -1) {
                std::cout << "[ATTACK] " << unit->get_name() << " (Atk: " << unit->get_attack()
                          << ") targets Row " << static_cast<int>(target_loc.type)
                          << " Index " << target_loc.index << std::endl;
                apply_damage(unit, target_loc);
            }
        }
    }
}

card_location combat_manager::find_best_target(row_side enemy_side, row_type attacker_row, stances stance) const {
    std::cout << "[DEBUG] Finding target for unit on row " << static_cast<int>(attacker_row)
              << " with stance " << static_cast<int>(stance) << std::endl;

    std::vector<row_type> priorities = { attacker_row };
    for (int i = 0; i < 5; ++i) {
        if (static_cast<row_type>(i) != attacker_row) priorities.push_back(static_cast<row_type>(i));
    }

    for (row_type target_row : priorities) {
        const auto& enemy_cards = game_board.get_row_cards(static_cast<int>(enemy_side), static_cast<int>(target_row));

        // stat based on stance
        value_type search_stat = (stance == stances::AGGRESSIVE) ? value_type::ARMOR : value_type::ATTACK;

        auto locations = game_board.get_max_value_locations_on_row(search_stat, enemy_side, target_row);

        for (const auto& loc : locations) {
            auto* target_unit = dynamic_cast<card_unit*>(enemy_cards[loc.index].get());

            // even if it's the 'max' value, don't return it if it's dying
            if (target_unit && !target_unit->is_dead()) {
                std::cout << "  - Targeting " << target_unit->get_name() << " at index " << loc.index << std::endl;
                return loc;
            }
        }
    }
    return {enemy_side, row_type::NONE, -1};
}

void combat_manager::apply_damage(const card_unit* attacker, const card_location& target_loc) const {
    if (!attacker) return;

    auto& target_row = game_board.get_row_cards(static_cast<int>(target_loc.side),
                                               static_cast<int>(target_loc.type));

    // index safety check
    if (target_loc.index < 0 || target_loc.index >= static_cast<int>(target_row.size())) {
        std::cout << "  [!] Attack failed: Target index " << target_loc.index << " is out of bounds.\n";
        return;
    }

    auto* target = dynamic_cast<card_unit*>(target_row[target_loc.index].get());

    if (!target) {
        std::cout << "  [!] Attack failed: Target at index " << target_loc.index << " is not a unit.\n";
        return;
    }

    // damage math
    int dmg = attacker->get_attack();
    if (target->is_dead()) return;

    if (target->get_armor() > 0) {
        target->change_armor(-dmg);
        std::cout << "[DEBUG] " << attacker->get_name() << " is dealing exactly " << dmg << " to " << target->get_name() << std::endl;
        if (target->get_armor() <= 0) {
            target->set_dead();
            std::cout << "    - [CRITICAL] " << target->get_name() << "'s armor shattered! Marked for death.\n";
        }
    } else {
        target->set_dead();
    }
}

void combat_manager::apply_damage_by_value(int dmg, const card_location& target_loc) const {

    auto& target_row = game_board.get_row_cards(static_cast<int>(target_loc.side),
                                               static_cast<int>(target_loc.type));

    if (target_loc.index < 0 || target_loc.index >= static_cast<int>(target_row.size())) return;

    auto* target = dynamic_cast<card_unit*>(target_row[target_loc.index].get());
    if (!target || target->is_dead()) return;

    std::cout << "[ABILITY] Dealing " << dmg << " damage to " << target->get_name() << "\n";

    // If unit has armor, it absorbs damage
    if (target->get_armor() > 0) {
        // If you want Armor to act as a "Shield" (absorbing the whole hit):
        target->change_armor(-dmg);
    } else {
        // If they have no armor, they are marked for death immediately by a high-damage strike
        target->set_dead();
    }

    // Post-damage check
    if (target->get_armor() <= 0) {
        target->set_dead();
        std::cout << "  - Target destroyed or armor shattered.\n";
    }
}