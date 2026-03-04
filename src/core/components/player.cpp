#include "core/components/player.h"
#include "core/components/board.h"
#include "core/components/ability/ability.h"
#include <iostream>

player::player(std::string c_name, deck c_deck)
    : name(std::move(c_name)), player_deck(std::move(c_deck)) {
    player_deck.shuffle();
}


void player::draw_card(int times) {
    for (int i = 0; i < times; ++i) {
        hand.push_back(player_deck.draw_top_card());
    }
}

void player::play_card(int index, board &b, row_side side, player& opponent, combat_manager& cm) {
    std::cout << "\n========================================================" << std::endl;
    std::cout << "[ACTION] Player " << (side == row_side::PLAYER ? "1" : "2")
              << " initiates play from hand index: " << index << std::endl;

    if (index < 0 || index >= hand.size()) return;

    auto it = hand.begin() + index;
    std::unique_ptr<card> card_to_play = std::move(*it);
    hand.erase(it);
    execute_play_card(std::move(card_to_play), b, side, opponent, cm);
    std::cout << "========================================================\n" << std::endl;
}

void player::execute_play_card(std::unique_ptr<card> card_ptr, board &b, row_side side, player& opponent, combat_manager& cm) {
    if (!card_ptr) return;

    std::cout << "  -> Deploying: " << card_ptr->get_name() << " [" << card_ptr->get_id() << "]" << std::endl;

    row_type target_row = row_type::SPECIAL;
    std::string type = card_ptr->get_card_type();

    if (type == "UNIT") {
        if (auto* unit_ptr = dynamic_cast<card_unit*>(card_ptr.get())) {
            std::string range = unit_ptr->get_range_type();
            if (range == "MELEE") target_row = row_type::MELEE;
            else if (range == "RANGED") target_row = row_type::RANGED;
            else if (range == "HEAVY") target_row = row_type::HEAVY;
            else if (range == "NET") target_row = row_type::NET;
            std::cout << "     | Row: " << range << " | Stance: " << static_cast<int>(unit_ptr->get_stance()) << std::endl;
        }
    }

    if (type == "SPECIAL") {
        target_row = row_type::SPECIAL;
        std::cout << "[DEBUG] Special row_type: " << static_cast<int>(target_row) << std::endl;
    }

    auto abilities = card_ptr->get_abilities();
    if (!abilities.empty()) {
        std::cout << "     | Triggering " << abilities.size() << " abilities..." << std::endl;

        auto* caster_ptr = dynamic_cast<card_unit*>(card_ptr.get());

        // Handle side-flipping for Spies first
        row_side placement_side = side;
        for (auto& ab : abilities) {
            if (ab && ab->get_type() == "SPY") {
                placement_side = (side == row_side::PLAYER) ? row_side::OPPONENT : row_side::PLAYER;
                std::cout << "     | [!] SPY DETECTED: Flipping to side " << static_cast<int>(placement_side) << std::endl;
                break;
            }
        }

        b.add_card(std::move(card_ptr), placement_side, target_row);

        ability_context ctx { cm, *this, opponent, caster_ptr };
        for (auto& ab : abilities) {
            if (ab) {
                // Use a special symbol for abilities
                std::cout << "     | [!] Executing: " << ab->get_id() << std::endl;
                ab->execute(ctx);
            }
        }
    } else {
        b.add_card(std::move(card_ptr), side, target_row);
    }
}

void player::add_to_graveyard(std::unique_ptr<card> card_ptr) {
    graveyard.push_back(std::move(card_ptr));
}

std::unique_ptr<card> player::pull_from_hand_by_id(const std::string& id) {
    for (auto it = hand.begin(); it != hand.end(); ++it) {
        if ((*it)->get_id() == id) {
            std::unique_ptr<card> found = std::move(*it);
            hand.erase(it);
            return found;
        }
    }
    return nullptr;
}

