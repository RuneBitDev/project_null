#ifndef PROJECT_NULL_TYPES_H
#define PROJECT_NULL_TYPES_H

// card specific types
enum class value_type {STRENGTH, ARMOR, ATTACK};

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
enum class strike_target { MAX_STRENGTH, MAX_ARMOR, MAX_ATTACK };
enum class strike_type {
    LETHAL,     // high single target damage
    BREAKER,    // some armor shredding
    SPLASH,     // horizontal damage
    FRAG,       // cross-pattern (front/back/left/right)
    BARRAGE,    // multi-target
    STATUS      // applies a modifier/debuff
};



struct card_location {
    row_side side;
    row_type type;
    int index = -1;

    bool is_valid() const { return index != -1; }
};
#endif //PROJECT_NULL_TYPES_H