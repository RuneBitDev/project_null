#ifndef PROJECT_NULL_UI_TYPES_H
#define PROJECT_NULL_UI_TYPES_H
#include "raylib.h"

enum button_type { CLICKABLE, HOLDABLE};

namespace card_conf {
    constexpr float WIDTH = 100.0f;
    constexpr float HEIGHT = 150.0f;
}

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
    int strength = 0;
    int armor = 0;
    int attack = 0;
    Color border_color = WHITE;
    bool face_up = false;

    bool is_highlighted = false;
};
#endif //PROJECT_NULL_UI_TYPES_H