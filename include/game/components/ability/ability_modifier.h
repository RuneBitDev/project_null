#ifndef PROJECT_NULL_ABILITY_WEATHER_H
#define PROJECT_NULL_ABILITY_WEATHER_H
#include "game/components/ability/ability.h"



class ability_modifier : public ability {
public:
    ability_modifier(std::string id, std::string name, const std::string &type, std::vector<ParamValue> params);
    void execute(ability_context &ctx) override;

private:
    row_type target_row;
    modifier_type m_type;
    int m_value = 0;
    bool clear_weather = false;

    void execute_mod(const ability_context &ctx) const;
    void execute_weather(const ability_context &ctx) const;
};

#endif //PROJECT_NULL_ABILITY_WEATHER_H