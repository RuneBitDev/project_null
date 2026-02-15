#include "core/components/ability/ability.h"

ability::ability(std::string id, std::string name, std::string type, std::vector<ParamValue> params)
    : ability_id(std::move(id)), ability_name(std::move(name)),
      ability_type(std::move(type)), ability_params(std::move(params)) {}

std::string ability::get_param_string(size_t index, const std::string& default_val) const {
    if (index < ability_params.size()) {
        if (const auto* val = std::get_if<std::string>(&ability_params[index])) {
            return *val;
        }
    }
    return default_val;
}

int ability::get_param_int(size_t index, int default_val) const {
    if (index < ability_params.size()) {
        if (const auto* val = std::get_if<int>(&ability_params[index])) {
            return *val;
        }
    }
    return default_val;
}