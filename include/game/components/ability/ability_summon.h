#ifndef PROJECT_NULL_ABILITY_SUMMON_H
#define PROJECT_NULL_ABILITY_SUMMON_H
#include "ability.h"

class ability_summon : public ability {
public:
    ability_summon(std::string id, std::string name, std::string type, std::vector<std::string> params);
    void execute(ability_context& ctx) override;

private:
    std::vector<std::string> target_ids;
};


#endif