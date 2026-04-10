#ifndef PROJECT_NULL_TYPES_H
#define PROJECT_NULL_TYPES_H
#include <string>

// card specific types
enum class value_type {STRENGTH, ARMOR, ATTACK};
enum class card_type { UNIT, SPECIAL, SUPPORT, LEADER, UNKNOWN };

// board specific types
enum class row_side { PLAYER, OPPONENT };
enum class row_type { MELEE, RANGED, HEAVY , NET , SPECIAL , NONE };

// ability specific types
enum class modifier_target {STRENGTH, ARMOR, ATTACK};
enum class modifier_type {
    ADD,        // buff
    SUBTRACT,   // debuff
    MULTIPLY,   // maybe bond ability
    SET,        // weather
    CLEAR       // clear weather
};
enum class strike_target { MAX, MIN };
enum class strike_type {
    LETHAL,     // high single target damage
    BREAKER,    // some armor shredding
    SPLASH,     // horizontal damage
    FRAG,       // cross-pattern (front/back/left/right)
    BARRAGE,    // multi-target
    STATUS      // applies a modifier/debuff
};
enum class summon_type { SUMMON, NECRO };

row_type string_to_row_type(const std::string& str);
modifier_target string_to_modifier_target(const std::string& str);
strike_type string_to_strike_type(const std::string& str);
value_type string_to_value_type(const std::string& str);
modifier_type string_to_modifier_type(const std::string& str);
strike_target string_to_strike_target(const std::string& str);
summon_type string_to_summon_type(const std::string& str);

struct card_location {
    row_side side;
    row_type type;
    int index = -1;

    bool is_valid() const { return index != -1; }
};
#endif //PROJECT_NULL_TYPES_H