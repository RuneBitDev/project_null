#ifndef PROJECT_NULL_FACTORY_H
#define PROJECT_NULL_FACTORY_H
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include "components/card.h"
#include "components/card_unit.h"
#include "components/deck.h"
#include "components/ability/ability.h"

static const std::string DB_PATH = "data.sqlite";
using ParamValue = std::variant<int, std::string>;

class factory {
public:
    bool load_master_data();
    deck build_deck(const std::string& faction);
    deck load_deck(const std::string& deck_id);

    const std::vector<card>& get_special_library() const;
    const std::vector<card_unit>& get_unit_library() const;

private:
    std::vector<card> special_library;
    std::vector<card_unit> unit_library;
    std::vector<std::shared_ptr<ability>> ability_library;
    std::vector<ParamValue> parse_params(const std::string& col_data);
};

#endif //PROJECT_NULL_FACTORY_H