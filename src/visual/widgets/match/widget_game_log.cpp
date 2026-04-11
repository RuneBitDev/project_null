#include "core/game_log.h"
#include "../../../../include/visual/widgets/match/widget_game_log.h"

#include <iostream>

#include "visual/ui_util.h"


void widget_game_log::update(float dt) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(render_config::get_virtual_mouse(), get_toggle_bounds())) {
            is_collapsed = !is_collapsed;
            scroll_offset = 0;
        }
    }

    // handle scrolling
    if (!is_collapsed) {
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            scroll_offset += static_cast<int>(wheel);

            int total_logs = static_cast<int>(game_log::log.size());
            int max_visible = 30;

            // can't scroll past the beginning or the end
            if (scroll_offset < 0) scroll_offset = 0;
            if (scroll_offset > (total_logs - max_visible)) {
                scroll_offset = total_logs - max_visible;
            }
            if (scroll_offset < 0) scroll_offset = 0;
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
        DrawRectangleRec(btn, Fade(GRAY, 0.2f));
        DrawRectangleLinesEx(btn, 1.0f, GOLD);
        ui::draw_text("LOG [+]", {btn.x + (btn.width/2), btn.y + (btn.height/2 - 8)}, 20, GOLD, true);

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
        ui::draw_text("COMBAT LOG [-]", {current_x + 10, y + 7}, 20, GOLD);
        DrawLine(current_x, y + 30, current_x + w, y + 30, GOLD);

        int total_logs = static_cast<int>(game_log::log.size());
        int max_render = 27;
        float text_y = (current_bounds.y + current_bounds.height) - 25;

        // scrolling logic
        if (total_logs > 0) {
            int count = 0;
            auto it = game_log::log.rbegin();

            std::advance(it, std::min(static_cast<int>(scroll_offset), static_cast<int>(game_log::log.size()) - 1));

            for (; it != game_log::log.rend(); ++it) {
                if (count >= max_render) break;
                if (it->message.empty()) {
                    text_y -= (font_size / 2.0f);
                    continue;
                }

                // format and draw
                std::string formattedMsg = (it->color.r == GOLD.r) ? it->message : "> " + it->message;
                ui::draw_text(formattedMsg.c_str(), {current_x + 15, text_y}, font_size, it->color);


                text_y -= (font_size + 6);
                count++;
            }
        }

        // scrollbar
        if (total_logs > max_render) {
            float bar_track_h = h - 40;
            float track_y = y + 35;
            float scroll_pct = static_cast<float>(scroll_offset) / (total_logs - max_render);
            float handle_h = 30.0f;
            float handle_y = track_y + ((1.0f - scroll_pct) * (bar_track_h - handle_h));

            // track
            DrawRectangle(current_x + bounds.width - 6, track_y, 2, bar_track_h, Fade(GRAY, 0.3f));

            // thumb
            Rectangle thumb = { current_x + bounds.width - 10, handle_y, 8, handle_h };
            DrawRectangleRec(thumb, GOLD);
            DrawRectangleLinesEx(thumb, 1, Fade(WHITE, 0.5f));
        }
    }
}

Rectangle widget_game_log::get_toggle_bounds() const {
    auto screen_w = static_cast<float>(GetScreenWidth());

    if (is_collapsed) {
        float btn_h = 30.0f;
        float btn_w = 120.0f;
        return { screen_w - btn_w, bounds.y, btn_w, btn_h };
    } else {
        float current_x = screen_w - bounds.width;
        return { current_x, bounds.y, bounds.width, 30.0f };
    }
}