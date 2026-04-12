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
    deck load_deck(const std::string& deck_id);

    const std::vector<std::unique_ptr<card>>& get_card_library() const { return card_library; }

private:
    std::vector<std::unique_ptr<card>> card_library;
    std::vector<std::shared_ptr<ability>> ability_library;
    std::vector<ParamValue> parse_params(const std::string& col_data);
    card_type get_card_type(const std::string& col_data);
};

#endif //PROJECT_NULL_FACTORY_H