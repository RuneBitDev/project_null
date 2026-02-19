#include "visual/widgets/widget_card.h"
#include "visual/render_config.h"

widget_card::widget_card(const card* c_ptr, Rectangle bounds, ui_card s)
    : card_data(c_ptr), base_bounds(bounds), current_bounds(bounds), state(s) {}

void widget_card::draw() const {

    if (hovered) {
        DrawRectangleRec(current_bounds, BLACK);
    }

    DrawRectangleLinesEx(current_bounds, hovered ? 3 : 2, state.border_color);

    if (state.face_up) {


        if (card_data->get_card_type() == "UNIT") {
            float circle_radius = hovered ? 18.0f : 15.0f;
            float circle_x = current_bounds.x + (hovered ? 25.0f : 20.0f);
            float circle_y = current_bounds.y + (hovered ? 25.0f : 20.0f);
            // draw Circles
            DrawCircleLines(circle_x, circle_y, circle_radius, GREEN);
            DrawCircleLines(circle_x, circle_y + 30, circle_radius, BLUE);
            DrawCircleLines(circle_x, circle_y + 60, circle_radius, RED);

            // draw numbers
            int font_size = hovered ? 20 : 15;
            DrawText(std::to_string(state.strength).c_str(), circle_x - (font_size/3), circle_y - (font_size/2), font_size, GREEN);
            DrawText(std::to_string(state.armor).c_str(), circle_x - (font_size/3), circle_y - (font_size/2) + 30, font_size, BLUE);
            DrawText(std::to_string(state.attack).c_str(), circle_x - (font_size/3), circle_y - (font_size/2) + 60, font_size, RED);

        }

        // Name rendering
        int name_size = hovered ? 12 : 10;
        int textWidth = MeasureText(card_data->get_name().c_str(), name_size);
        float posX = current_bounds.x + (current_bounds.width / 2.0f) - (static_cast<float>(textWidth) / 2.0f);
        DrawText(card_data->get_name().c_str(), static_cast<int>(posX),
                 static_cast<int>(current_bounds.y) + static_cast<int>(current_bounds.height) - (hovered ? 30 : 25),
                 name_size, GREEN);
    }
}

void widget_card::update() {
    Vector2 mouse_pos = render_config::get_virtual_mouse();
    hovered = CheckCollisionPointRec(mouse_pos, base_bounds);

    if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        triggered = true;
    }
}
