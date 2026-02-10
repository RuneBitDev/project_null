#ifndef PROJECT_NULL_ABILITY_SPY_H
#define PROJECT_NULL_ABILITY_SPY_H
#include "core/components/ability/ability.h"

class ability_spy : public ability {
public:
    ability_spy(std::string id, std::string name, std::string type, std::vector<ParamValue> params);
    void execute(ability_context &ctx) override;
private:
    int number_of_cards;

};

#endif //PROJECT_NULL_ABILITY_SPY_H