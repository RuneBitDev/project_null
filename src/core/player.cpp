#include "game/player.h"

player::player(std::string c_name, deck c_deck)
    : name(std::move(c_name)), player_deck(std::move(c_deck)) {
    player_deck.shuffle();
    draw_card(10);
}

void player::display_hand() const {
    std::cout << name << "'s Hand: ";
    if (hand.empty()) {
        std::cout << "(Empty)";
    } else {
        for (size_t i = 0; i < hand.size(); ++i) {
            std::cout << i << ": " << hand[i]->get_name() << " | ";
        }
    }
     std::cout << "\nDeck remains: " << player_deck.get_size() << " | Graveyard: " << std::endl;
}

void player::draw_card(int times) {
    for (int i = 0; i < times; ++i) {
        hand.push_back(player_deck.draw_top_card());
    }
}

void player::play_card(int index, board &b) {
    if (index < 0 || index >= hand.size()) return;

    card_unit* unit_ptr = dynamic_cast<card_unit*>(hand[index].get());

    if (unit_ptr) {
        row_type target_row = row_type::MELEE;
        std::string range = unit_ptr->get_range_type();

        if (range == "RANGED") target_row = row_type::RANGED;
        else if (range == "HEAVY") target_row = row_type::HEAVY;
        else if (range == "NET") target_row = row_type::NET;

        b.add_card(*unit_ptr, row_side::PLAYER, target_row);

        hand.erase(hand.begin() + index);
    }

}

