#include "visual/ui_element.h"

bool ui_element::button(float x, float y, float w, float h) {
    Rectangle startBtn = { x, y, w, h};

    if (CheckCollisionPointRec(GetMousePosition(), startBtn)) {
        return IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    return false;
}
