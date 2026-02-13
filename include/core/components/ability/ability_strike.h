#ifndef PROJECT_NULL_ABILITY_REMOVAL_H
#define PROJECT_NULL_ABILITY_REMOVAL_H
#include "core/components/ability/ability.h"

class ability_strike : public ability {
public:
    ability_strike(std::string id, std::string name, const std::string &type, std::vector<ParamValue> params);
    void execute(ability_context &ctx) override;

private:
    strike_type s_type;
    strike_target s_target;
    int damage_amount;


    void execute_lethal(ability_context &ctx);
    void execute_splash(ability_context &ctx);


};

#endif //PROJECT_NULL_ABILITY_REMOVAL_H