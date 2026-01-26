#ifndef PROJECT_NULL_ABILITY_WEATHER_H
#define PROJECT_NULL_ABILITY_WEATHER_H
#include "game/components/ability/ability.h"

class ability_weather : public ability {
public:
    ability_weather(std::string id, std::string name, std::string type, std::vector<ParamValue> params);
    void execute(ability_context &ctx) override;

private:
    row_type target_row_type;
    void apply_weather_to_side(ability_context &ctx, row_side side);
};

#endif //PROJECT_NULL_ABILITY_WEATHER_H