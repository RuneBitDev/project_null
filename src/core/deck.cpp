#include <algorithm>
#include <random>
#include "game/deck.h"
#include "game/game_config.h"


deck::deck(std::unique_ptr<card> c_leader, std::vector<std::unique_ptr<card>> c_cards)
    : leader(std::move(c_leader)), cards(std::move(c_cards)) {}

bool deck::is_valid() {
    int count_units = 0;
    int count_specials = 0;

    for (auto& c : cards) {
        std::string type = c->get_card_type();
        if (type == "UNIT") {
            count_units++;
        } else if (type == "SPECIAL") {
            count_specials++;
        }
    }

    return (count_specials >= game_config::deck_rules::MIN_SPECIAL_CARDS && count_units >= game_config::deck_rules::MIN_UNIT_CARDS);
}

std::unique_ptr<card> deck::draw_top_card() {
    if (cards.empty()) return nullptr;

    std::unique_ptr<card> top_card = std::move(cards.back());
    cards.pop_back();
    return top_card;
}

int deck::get_size() const{
    return cards.size();
}

void deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

