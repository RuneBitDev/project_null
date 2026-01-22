#include "../../../include/game/components/player.h"

player::player(std::string c_name, deck c_deck)
    : name(std::move(c_name)), player_deck(std::move(c_deck)) {
    player_deck.shuffle();
    draw_card(10);
}


void player::draw_card(int times) {
    for (int i = 0; i < times; ++i) {
        hand.push_back(player_deck.draw_top_card());
    }
}

void player::play_card(int index, board &b, row_side side) {
    if (index < 0 || index >= hand.size()) return;

    if (auto* card_ptr = hand[index].get()) {
        std::string type = card_ptr->get_card_type();
        row_type target_row = row_type::SPECIAL;
        if (type == "UNIT") {
            std::string range = card_ptr->get_range_type();

            if (range == "MELEE") target_row = row_type::MELEE;
            else if (range == "RANGED") target_row = row_type::RANGED;
            else if (range == "HEAVY") target_row = row_type::HEAVY;
            else if (range == "NET") target_row = row_type::NET;
        }


        auto it = hand.begin() + index;
        std::unique_ptr<card> card_to_play = std::move(*it);
        hand.erase(it);

        b.add_card(std::move(card_to_play), side, target_row);
    }

}

