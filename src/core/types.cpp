#include "core/types.h"
#include <algorithm>

std::string to_upper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

row_type string_to_row_type(const std::string& str) {
    std::string s = to_upper(str);
    if (s == "MELEE")  return row_type::MELEE;
    if (s == "RANGED") return row_type::RANGED;
    if (s == "HEAVY")  return row_type::HEAVY;
    if (s == "NET")    return row_type::NET;
    return row_type::NONE;
}

modifier_target string_to_modifier_target(const std::string& str) {
    std::string s = to_upper(str);
    if (s == "ST" || s == "STRENGTH") return modifier_target::STRENGTH;
    if (s == "AR" || s == "ARMOR")    return modifier_target::ARMOR;
    if (s == "AT" || s == "ATTACK")   return modifier_target::ATTACK;
    return modifier_target::STRENGTH;
}

strike_type string_to_strike_type(const std::string& str) {
    std::string s = to_upper(str);
    if (s == "LETHAL")  return strike_type::LETHAL;
    if (s == "SPLASH")  return strike_type::SPLASH;
    if (s == "FRAG")    return strike_type::FRAG;
    if (s == "BARRAGE") return strike_type::BARRAGE;
    if (s == "BREAKER") return strike_type::BREAKER;
    return strike_type::LETHAL;
}

value_type string_to_value_type(const std::string& str) {
    std::string s = to_upper(str);
    if (s == "ARMOR")    return value_type::ARMOR;
    if (s == "ATTACK")   return value_type::ATTACK;
    return value_type::STRENGTH;
}

modifier_type string_to_modifier_type(const std::string& str) {
    std::string s = to_upper(str);
    if (s == "ADD")      return modifier_type::ADD;
    if (s == "SUBTRACT" || s == "SUB") return modifier_type::SUBTRACT;
    if (s == "SET")      return modifier_type::SET;
    if (s == "CLEAR")    return modifier_type::CLEAR;
    return modifier_type::ADD;
}

strike_target string_to_strike_target(const std::string& str) {
    return (to_upper(str) == "MIN") ? strike_target::MIN : strike_target::MAX;
}

summon_type string_to_summon_type(const std::string& str) {
    std::string s = to_upper(str);
    if (s == "NECRO") return summon_type::NECRO;
    return summon_type::SUMMON; // Default to SUMMON if not Necro
}
