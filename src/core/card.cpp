#include "../../include/game/card.h"

#include <iostream>
#include <ostream>
#include <string>

card::card(int id, std::string name, std::string faction)
    : id(id), name(name), faction(faction) {}

void card::display() const {
    std::cout << this->name << ": " << this->id << std::endl;
}

int card::get_id() const {
    return id;
}

std::string card::get_name() const {
    return name;
}