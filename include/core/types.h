#ifndef PROJECT_NULL_TYPES_H
#define PROJECT_NULL_TYPES_H



enum class row_side { PLAYER, OPPONENT };
enum class row_type { MELEE = 0, RANGED = 1, HEAVY = 2, NET = 3, SPECIAL = 4, NONE = 5};
enum class modifier_type {ADD, SUBTRACT, MULTIPLY, SET, CLEAR};
enum class modifier_target {STRENGTH, ARMOR, ATTACK};
enum class strike_type {LETHAL, BREAKER, SPLASH, FINISH};
enum class value_type {STRENGTH, ARMOR, ATTACK};

struct card_location {
    row_side side;
    row_type type;
    int index = -1;

    bool is_valid() const { return index != -1; }
};
#endif //PROJECT_NULL_TYPES_H