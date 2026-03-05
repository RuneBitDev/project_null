#include "visual/widgets/widget_button.h"

#include <cmath>

#include "visual/render_config.h"

void widget_button::init_button(const char* text, button_type b_type, int key, Rectangle bounds) {
    this->text = text;
    this->b_type = b_type;
    this->shortcut_key = key;
    this->bounds = bounds;
}

void widget_button::update(float dt) {
    if (!enabled) {
        is_hovered = false;
        is_down = false;
        hold_progress = 0.0f;
        triggered = false;
        return;
    }
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
    float pulse = (sinf(GetTime() * 2.0f) * 0.5f + 0.5f);
    Color themeColor = is_hovered ? LIME : GREEN;
    if (!enabled) {
        themeColor = DARKGRAY;
    }

    float alpha = is_hovered ? 0.3f : 0.1f;

    float x = bounds.x;
    float y = bounds.y;
    float w = bounds.width;
    float h = bounds.height;

    DrawRectangleRec(bounds, Fade(BLACK, 0.8f));
    DrawRectangleGradientV(x, y, w, h,Fade(themeColor, alpha), Fade(BLACK, 0.0f));

    DrawRectangleLinesEx(bounds, is_hovered ? 2.0f : 1.0f, Fade(themeColor, 0.4f + (pulse * 0.3f)));

    // cyber jitter on hover
    if (text) {
        int fontSize = 24;
        int textWidth = MeasureText(text, fontSize);
        Vector2 textPos = {x + (w / 2 - textWidth / 2), y + (h / 2 - fontSize / 2)};

        if (is_hovered) {
            DrawText(text, textPos.x + 1, textPos.y + 1, fontSize, RED); // Ghosting effect
        }
        DrawText(text, textPos.x, textPos.y, fontSize, themeColor);
    }

    // loading bar
    if (b_type == HOLDABLE && hold_progress > 0.0f) {
        float progressWidth = (w - 10) * hold_progress;
        Rectangle progressRect = { x + 5, y + h - 8, progressWidth, 4 };

        DrawRectangleRec(progressRect, themeColor);
        DrawCircle(progressRect.x + progressWidth, progressRect.y + 2, 4, WHITE);
    }
}
