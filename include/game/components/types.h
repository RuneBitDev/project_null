#ifndef PROJECT_NULL_TYPES_H
#define PROJECT_NULL_TYPES_H

enum class row_side { PLAYER, OPPONENT };
enum class row_type { MELEE = 0, RANGED = 1, HEAVY = 2, NET = 3, SPECIAL = 4};
enum class modifier_type {ADD, SUBTRACT, MULTIPLY, SET, CLEAR};
enum class modifier_target {STRENGTH, ARMOR, ATTACK};
enum class strike_type {LETHAL, BREAKER, SPLASH, FINISH};

#endif //PROJECT_NULL_TYPES_H