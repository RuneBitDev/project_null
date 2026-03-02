#ifndef PROJECT_NULL_UI_TYPES_H
#define PROJECT_NULL_UI_TYPES_H
#include "raylib.h"
#include "core/match_manager.h"

enum button_type { CLICKABLE, HOLDABLE};
enum class card_position { DECK, HAND, ROW, GRAVEYARD };
enum class popup_type { BANNER };


struct card_context {
    int strength = 0;
    int virtual_strength = 0;
    int armor = 0;
    int attack = 0;
    Color border_color = WHITE;
    Rectangle card_bounds{};
    card_position position;

    bool face_up = false;
};

struct player_context {
    current_state current_state;
};

struct match_context {
    match_status m_status;
    current_state current_state;
};



#endif //PROJECT_NULL_UI_TYPES_H