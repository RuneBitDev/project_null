#ifndef PROJECT_NULL_FACTORY_H
#define PROJECT_NULL_FACTORY_H
#include <string>
#include <vector>
#include "card.h"
#include "card_unit.h"
#include "deck.h"

class factory {
public:
    bool load_master_data(const std::string& filepath);
    deck build_deck(const std::string& faction);

    const std::vector<card>& get_special_library() const;
    const std::vector<card_unit>& get_unit_library() const;

private:
    std::vector<card> special_library;          // Leader and Special cards
    std::vector<card_unit> unit_library;        // Unit cards


};

#endif //PROJECT_NULL_FACTORY_H