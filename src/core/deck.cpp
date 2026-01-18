#include <iostream>
#include <algorithm>
#include <random>
#include "game/deck.h"
#include "game/game_config.h"

deck::deck(std::unique_ptr<card> c_leader, std::vector<std::unique_ptr<card>> c_cards)
    : leader(std::move(c_leader)), cards(std::move(c_cards)) {}

bool deck::is_valid() {


    return true;
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

