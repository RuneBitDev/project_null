#include "visual/renderer.h"
#include "visual/render_config.h"
#include "visual/ui_element.h"



void renderer::draw_start_screen() {
    ClearBackground(BLACK);

    draw_text_centered("PROJECT NULL", 300, 80, GREEN);
    draw_text_centered("PROJECT NULL", 500, 40, GREEN);
    draw_text_centered("Press Enter", 1000, 30, GREEN);
}

void renderer::draw_menu() {
    ClearBackground(BLACK);
    draw_button(render_config::ui::START_BUTTON);
}

void renderer::draw_game(const render_context& ctx) {
    ClearBackground(BLACK);
    board_view.update_from_game(ctx.b);
    hand_view.update_from_player(ctx.p1);
    graveyard_view_p1.update_from_player(ctx.p1);
    graveyard_view_p2.update_from_player(ctx.p2);

    board_view.update();
    hand_view.update();
    graveyard_view_p1.update();
    graveyard_view_p2.update();

    board_view.draw();
    hand_view.draw();
    graveyard_view_p1.draw();
    graveyard_view_p2.draw();

    draw_button(render_config::ui::PASS_BUTTON);

}


// ---------------------------------------------------------------------
// HELPER FUNCTIONS
// ---------------------------------------------------------------------


void renderer::draw_button(button& btn) {
    ui_element::update_button(btn);
    Color tint = btn.is_hovered ? LIME : BLACK;

    DrawRectangleRec(btn.bounds, tint);
    DrawRectangleLinesEx(btn.bounds, 2 , GREEN);

    if (btn.text) {
        int fontSize = 40;
        int textWidth = MeasureText(btn.text, fontSize);
        DrawText(btn.text,
            static_cast<int>(btn.bounds.x + (btn.bounds.width / 2 - textWidth / 2)),
            static_cast<int>(btn.bounds.y + (btn.bounds.height / 2 - fontSize / 2)),
            fontSize, GREEN);
    }

    if (btn.type == HOLDABLE) {
        float progress_width = btn.bounds.width * btn.hold_progress;
        Rectangle fill_rect = {btn.bounds.x, btn.bounds.y, progress_width, btn.bounds.height};
        DrawRectangleRec(fill_rect, GREEN);
    }

}

void renderer::draw_text_centered(const char* text, int y, int size, Color color) {
    int width = MeasureText(text, size);
    DrawText(text, render_config::VIRTUAL_WIDTH / 2 - width / 2, y, size, color);
}

void renderer::draw_text_in_rect(const char* text, Rectangle rect, int y_offset, int size, Color color) {
    int textWidth = MeasureText(text, size);
    float posX = rect.x + (rect.width / 2.0f) - (static_cast<float>(textWidth) / 2.0f);

    DrawText(text, static_cast<int>(posX), static_cast<int>(rect.y) + y_offset, size, color);
}
