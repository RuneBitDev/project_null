#include "core/game_log.h"
#include "visual/widgets/widget_game_log.h"

#include <iostream> // For debugging



void widget_game_log::update(float dt) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = render_config::get_virtual_mouse();
        Rectangle hitzone = get_toggle_bounds();

        if (CheckCollisionPointRec(mousePos, hitzone)) {
            is_collapsed = !is_collapsed;
        }
    }
}

void widget_game_log::draw() const {
    auto screen_w = static_cast<float>(GetScreenWidth());
    float x = bounds.x;
    float y = bounds.y;
    float w = bounds.width;
    float h = bounds.height;

    if (is_collapsed) { // expand button
        Rectangle btn = get_toggle_bounds();
        DrawRectangleRec({btn.x + 2, btn.y + 2, btn.width, btn.height}, Fade(BLACK, 0.4f));
        DrawRectangleRec(btn, Fade(BLACK, 0.8f));
        DrawRectangleLinesEx(btn, 1.0f, GOLD);
        const char* txt = "LOG [+]";
        int tw = MeasureText(txt, 16);
        DrawText(txt, btn.x + (btn.width/2 - tw/2), btn.y + (btn.height/2 - 8), 16, GOLD);
    }
    else { // full console
        float current_x = screen_w - w;
        Rectangle current_bounds = { current_x, y, w, h };

        // main panel
        DrawRectangleRec(current_bounds, Fade(BLACK, 0.9f));
        DrawRectangleLinesEx(current_bounds, 1.0f, Fade(RAYWHITE, 0.4f));
        // header
        Rectangle header = { current_x, y, w, 30 };
        DrawRectangleRec(header, Fade(GRAY, 0.2f));
        DrawText("COMBAT LOG [-]", current_x + 10, y + 7, 16, GOLD);
        DrawLine(current_x, y + 30, current_x + w, y + 30, GOLD);

        int max_visible = 12;
        int count = 0;
        float text_y = (current_bounds.y + current_bounds.height) - 25;
        float padding_left = 15.0f;

        for (auto it = game_log::log.rbegin(); it != game_log::log.rend(); ++it) {
            if (count >= max_visible) break;

            DrawCircle(current_x + 6, text_y + (font_size/2), 2, Fade(it->color, 0.5f));
            DrawText(it->message.c_str(), current_x + padding_left, text_y, font_size, it->color);
            text_y -= (font_size + 6);
            count++;
        }
    }
}

Rectangle widget_game_log::get_toggle_bounds() const {
    auto screen_w = static_cast<float>(GetScreenWidth());
    float btn_w = 120.0f;
    float btn_h = 40.0f;

    if (is_collapsed) {
        return { screen_w - btn_w, bounds.y, btn_w, btn_h };
    } else {
        float current_x = screen_w - bounds.width;
        return { current_x, bounds.y, bounds.width, 30.0f };
    }
}