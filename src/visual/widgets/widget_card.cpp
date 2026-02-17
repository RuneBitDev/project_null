#include "visual/widgets/widget_card.h"

void widget_card::draw(const ui_card& card) const {
    if (hovered) {
        DrawRectangleRec(current_bounds, BLACK);
    }

    // Border color is now provided by the parent, no stance logic needed here
    DrawRectangleLinesEx(current_bounds, hovered ? 3 : 2, card.border_color);

    if (face_up) {
        float circle_radius = hovered ? 18.0f : 15.0f;
        float circle_x = current_bounds.x + (hovered ? 25.0f : 20.0f);
        float circle_y = current_bounds.y + (hovered ? 25.0f : 20.0f);

        // Draw Circles
        DrawCircleLines(circle_x, circle_y, circle_radius, GREEN);
        DrawCircleLines(circle_x, circle_y + 30, circle_radius, BLUE);
        DrawCircleLines(circle_x, circle_y + 60, circle_radius, RED);

        // Draw Numbers using the pre-calculated state
        int font_size = hovered ? 20 : 15;
        DrawText(std::to_string(card.strength).c_str(), circle_x - (font_size/3), circle_y - (font_size/2), font_size, GREEN);
        DrawText(std::to_string(card.armor).c_str(), circle_x - (font_size/3), circle_y - (font_size/2) + 30, font_size, BLUE);
        DrawText(std::to_string(card.attack).c_str(), circle_x - (font_size/3), circle_y - (font_size/2) + 60, font_size, RED);

        // Name rendering
        int name_size = hovered ? 12 : 10;
        int textWidth = MeasureText(card_ptr->get_name().c_str(), name_size);
        float posX = current_bounds.x + (current_bounds.width / 2.0f) - (static_cast<float>(textWidth) / 2.0f);
        DrawText(card_ptr->get_name().c_str(), static_cast<int>(posX),
                 static_cast<int>(current_bounds.y) + static_cast<int>(current_bounds.height) - (hovered ? 30 : 25),
                 name_size, GREEN);
    }
}