#include "visual/renderer.h"

void renderer::draw_start_screen() {
    ClearBackground(BLACK);
    draw_text_centered("PROJECT NULL", 150, 40, RAYWHITE);
    draw_text_centered("PROJECT NULL", 250, 20, GRAY);
}

void renderer::draw_menu() {
    ClearBackground(WHITE);
}

void renderer::draw_text_centered(const char* text, int y, int size, Color color) {
    int width = MeasureText(text, size);
    DrawText(text, GetScreenWidth() / 2 - width / 2, y, size, color);
}