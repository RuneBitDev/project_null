#ifndef PROJECT_NULL_UI_TYPES_H
#define PROJECT_NULL_UI_TYPES_H
#include "raylib.h"
#include "game/components/card.h"

enum button_type { CLICKABLE, HOLDABLE};

struct button {
    Rectangle bounds{};
    button_type type;
    int shortcut_key{};
    const char* text{};

    bool is_hovered = false;
    bool is_down = false;
    float hold_progress = 0.0f;
    bool triggered = false;
};

struct ui_card {
    Rectangle bounds{};
    // png
    bool is_hovered = false;
    bool triggered = false;
    bool is_face_up = false;
};
#endif //PROJECT_NULL_UI_TYPES_H