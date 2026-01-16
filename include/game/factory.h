#ifndef PROJECT_NULL_FACTORY_H
#define PROJECT_NULL_FACTORY_H
#include <string>
#include <vector>
#include "card.h"
#include "card_unit.h"


class factory {
    private:
    std::vector<card> card_library;         // Leader and Special cards
    std::vector<card_unit> unit_library;    // Unit cards


    public:
    bool load_master_data(const std::string& filepath);

    std::vector<card>& get_card_library();
    std::vector<card_unit>& get_unit_library();

};

#endif //PROJECT_NULL_FACTORY_H