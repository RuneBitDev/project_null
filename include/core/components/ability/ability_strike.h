#ifndef PROJECT_NULL_ABILITY_REMOVAL_H
#define PROJECT_NULL_ABILITY_REMOVAL_H
#include "core/components/ability/ability.h"

class ability_strike : public ability {
public:
    ability_strike(std::string id, std::string name, const std::string &type, const std::vector<ParamValue> &params);
    void execute(ability_context &ctx) override;

private:
    strike_type s_type;
    strike_target strike_target;
    value_type s_target_type;
    int damage_amount;


    void execute_lethal(const ability_context &ctx) const;
    void execute_splash(const ability_context &ctx) const;
    void execute_frag(const ability_context &ctx) const;
    void execute_barrage(ability_context &ctx);
    void execute_breaker(ability_context &ctx);
    void execute_status(ability_context &ctx);


};

#endif //PROJECT_NULL_ABILITY_REMOVAL_H