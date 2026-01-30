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
    Rectangle bounds{};
    // png
    bool is_hovered = false;
    bool triggered = false;
    bool is_face_up = false;

    ui_card(float x, float y, bool c_is_face_up) : bounds(x, y, card_conf::WIDTH, card_conf::HEIGHT), is_face_up(c_is_face_up) {}
};
#endif //PROJECT_NULL_UI_TYPES_H