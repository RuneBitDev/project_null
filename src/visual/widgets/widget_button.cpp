#include "visual/widgets/widget_button.h"

#include "visual/render_config.h"

void widget_button::init_button(const char* text, button_type b_type, int key, Rectangle bounds) {
    this->text = text;
    this->b_type = b_type;
    this->shortcut_key = key;
    this->bounds = bounds;
}

void widget_button::update(float dt) {
    Vector2 mouse_pos = render_config::get_virtual_mouse();
    is_hovered = CheckCollisionPointRec(mouse_pos, bounds);
    triggered = false;

    bool mouse_interact = (is_hovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON));
    bool key_interact = (shortcut_key != 0 && IsKeyDown(shortcut_key));

    if (mouse_interact || key_interact) {
        is_down = true;
        if (b_type == HOLDABLE) {
            hold_progress += dt;
            if (hold_progress >= 1.0f) {
                hold_progress = 1.0f;
                triggered = true;
            }
        }
    } else {
        if (is_down && b_type == CLICKABLE && (is_hovered || key_interact)) {
            triggered = true;
        }

        is_down = false;
        hold_progress = 0.0f;
    }
}

void widget_button::draw() const {
    Color tint = is_hovered ? LIME : BLACK;

    DrawRectangleRec(bounds, tint);
    DrawRectangleLinesEx(bounds, 2 , GREEN);

    if (text) {
        int fontSize = 40;
        int textWidth = MeasureText(text, fontSize);
        DrawText(text,
            static_cast<int>(bounds.x + (bounds.width / 2 - textWidth / 2)),
            static_cast<int>(bounds.y + (bounds.height / 2 - fontSize / 2)),
            fontSize, GREEN);
    }

    if (b_type == HOLDABLE) {
        float progress_width = bounds.width * hold_progress;
        Rectangle fill_rect = {bounds.x, bounds.y, progress_width, bounds.height};
        DrawRectangleRec(fill_rect, GREEN);
    }
}
