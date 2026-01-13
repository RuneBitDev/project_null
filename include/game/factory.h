#ifndef PROJECT_NULL_CARD_MANAGER_H
#define PROJECT_NULL_CARD_MANAGER_H
#include <string>
#include <vector>
#include "card_unit.h"
#include "card_effect.h"

class factory {
    private:
    std::vector<card_unit> unit_library;
    std::vector<card_effect> effect_library;

    public:
    bool load_master_data(const std::string& filepath);



};

#endif //PROJECT_NULL_CARD_MANAGER_H