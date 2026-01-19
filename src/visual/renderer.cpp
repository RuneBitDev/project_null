#include "visual/renderer.h"

void renderer::draw_start_screen() {
    ClearBackground(BLACK);
    draw_text_centered("PROJECT NULL", 150, 40, RAYWHITE);
    draw_text_centered("PROJECT NULL", 250, 20, GRAY);
}

void renderer::draw_menu() {
    ClearBackground(WHITE);

    draw_button((float)GetScreenWidth()/2 - 100, 300, 200, 50, "START GAME");
}

void renderer::draw_button(float x, float y, float w, float h, const char *text) {
    Rectangle rect = {x, y, w, h};
    bool hovered = CheckCollisionPointRec(GetMousePosition(), rect);
    Color tint = hovered ? LIGHTGRAY : GRAY;

    DrawRectangleRec(rect, tint);
    DrawRectangleLinesEx(rect, 2 , RAYWHITE);

    int fontSize = 20;
    int textWidth = MeasureText(text, fontSize);
    DrawText(text,
        x + (w / 2 - textWidth / 2),
        y + (h / 2 - fontSize / 2),
        fontSize, RAYWHITE);
}

void renderer::draw_text_centered(const char* text, int y, int size, Color color) {
    int width = MeasureText(text, size);
    DrawText(text, GetScreenWidth() / 2 - width / 2, y, size, color);
}