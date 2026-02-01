#include "game/components/player.h"
#include "game/components/board.h"
#include "game/components/ability/ability.h"
#include <iostream>

#include "game/game_config.h"

player::player(std::string c_name, deck c_deck)
    : name(std::move(c_name)), player_deck(std::move(c_deck)) {
    player_deck.shuffle();
    draw_card(game_config::player_config::HAND_SIZE);
}


void player::draw_card(int times) {
    for (int i = 0; i < times; ++i) {
        hand.push_back(player_deck.draw_top_card());
    }
}

void player::play_card(int index, board &b, row_side side, player& opponent) {
    if (index < 0 || index >= hand.size()) return;

    auto it = hand.begin() + index;
    std::unique_ptr<card> card_to_play = std::move(*it);
    hand.erase(it);
    std::cout << "\nDEBUG: Playing card : " << card_to_play->get_name() << std::endl;
    execute_play_card(std::move(card_to_play), b, side, opponent);
}

void player::execute_play_card(std::unique_ptr<card> card_ptr, board &b, row_side side, player& opponent) {
    if (!card_ptr) {
        std::cout << "[DEBUG] execute_play_card received null card_ptr!" << std::endl;
        return;
    }

    std::cout << "[DEBUG] Playing card: " << card_ptr->get_name() << " (ID: " << card_ptr->get_id() << ")" << std::endl;

    row_type target_row = row_type::SPECIAL;
    std::string type = card_ptr->get_card_type();

    if (type == "UNIT") {
        if (auto* unit_ptr = dynamic_cast<card_unit*>(card_ptr.get())) {
            std::string range = unit_ptr->get_range_type();
            if (range == "MELEE") target_row = row_type::MELEE;
            else if (range == "RANGED") target_row = row_type::RANGED;
            else if (range == "HEAVY") target_row = row_type::HEAVY;
            else if (range == "NET") target_row = row_type::NET;
            std::cout << "[DEBUG] Unit range: " << range << " -> Row: " << static_cast<int>(target_row) << std::endl;
        }
    }

    if (type == "SPECIAL") {
        target_row = row_type::SPECIAL;
        std::cout << "[DEBUG] Special row_type: " << static_cast<int>(target_row) << std::endl;
    }

    row_side placement_side = side;
    auto abilities = card_ptr->get_abilities();
    std::cout << "[DEBUG] Card has " << abilities.size() << " abilities to trigger." << std::endl;

    for (auto& ab : abilities) {
        if (ab && ab->get_type() == "SPY") {
            // Flip the side: if I play it, it goes to the OPPONENT
            placement_side = (side == row_side::PLAYER) ? row_side::OPPONENT : row_side::PLAYER;
            break;
        }
    }

    b.add_card(std::move(card_ptr), placement_side, target_row);

    ability_context ctx { b, *this, opponent };
    for (auto& ab : abilities) {
        if (ab) {
            std::cout << "[DEBUG] Triggering ability type: " << ab->get_id() << std::endl;
            ab->execute(ctx);
        }
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

