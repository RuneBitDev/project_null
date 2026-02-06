#ifndef PROJECT_NULL_ABILITY_REMOVAL_H
#define PROJECT_NULL_ABILITY_REMOVAL_H
#include "game/components/ability/ability.h"

class ability_strike : public ability {
public:
    ability_strike(std::string id, std::string name, const std::string &type, std::vector<ParamValue> params);
    void execute(ability_context &ctx) override;

private:
    int damage_amount;
    strike_type s_type;


};

#endif //PROJECT_NULL_ABILITY_REMOVAL_H