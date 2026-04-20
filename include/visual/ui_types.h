#ifndef PROJECT_NULL_UI_TYPES_H
#define PROJECT_NULL_UI_TYPES_H
#include "raylib.h"
#include "core/match_manager.h"

enum button_type { CLICKABLE, HOLDABLE};
enum class card_position { DECK, HAND, ROW, GRAVEYARD, POOL };
enum class popup_type { BANNER };
enum class card_detail { MIN, MAX, MATCH };


struct card_context {
    Rectangle card_bounds;
    card_position position;
    card_detail detail = card_detail::MATCH;
    bool face_up = false;
    int strength, virtual_strength, armor, attack;
    Color border_color = WHITE;
};

struct player_context {
    current_state c_state;
    int total_score_p1;
    int total_score_p2;
};

struct match_context {
    match_status m_status;
    current_state c_state;
};



#endif //PROJECT_NULL_UI_TYPES_H