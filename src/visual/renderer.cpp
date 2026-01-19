#include "visual/renderer.h"

void renderer::draw_start_screen() {
    ClearBackground(BLACK);
    draw_text_centered("PROJECT NULL", 150, 40, RAYWHITE);
    draw_text_centered("PROJECT NULL", 250, 20, GRAY);
}

void renderer::draw_menu() {
    ClearBackground(WHITE);

    Rectangle startBtn = { (float)GetScreenWidth()/2 - 100, 300, 200, 50};
    bool isHovered = CheckCollisionPointRec(GetMousePosition(), startBtn);

    draw_button(startBtn, "START GAME", isHovered);
}

void renderer::draw_button(Rectangle bounds, const char *text, bool hovered) {

    Color tint = hovered ? LIGHTGRAY : GRAY;

    DrawRectangleRec(bounds, tint);
    DrawRectangleLinesEx(bounds, 2 , RAYWHITE);

    int fontSize = 20;
    int textWidth = MeasureText(text, fontSize);
    DrawText(text,
        bounds.x + (bounds.width / 2 - textWidth / 2),
        bounds.y + (bounds.height / 2 - fontSize / 2),
        fontSize, RAYWHITE);
}

void renderer::draw_text_centered(const char* text, int y, int size, Color color) {
    int width = MeasureText(text, size);
    DrawText(text, GetScreenWidth() / 2 - width / 2, y, size, color);
}