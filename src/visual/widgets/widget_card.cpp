#include "visual/widgets/widget_card.h"

#include "core/components/card_unit.h"
#include "visual/render_config.h"

widget_card::widget_card(const card* c_ptr, Rectangle bounds, ui_card s)
    : card_data(c_ptr), base_bounds(bounds), current_bounds(bounds), state(s) {}

void widget_card::draw() const {
    Color border_color = state.border_color;

    if (auto unit = dynamic_cast<const card_unit*>(card_data)) {
        switch (unit->get_stance()) {
            case stances::AGGRESSIVE:  border_color = RED;      break;
            case stances::SUPPRESSIVE: border_color = GREEN;    break;
            case stances::DEFENSIVE:   border_color = BLUE;     break;
        }
    }

    DrawRectangleLinesEx(current_bounds, hovered ? 3 : 2, border_color);

    if (state.face_up) {

        if (card_data->get_card_type() == "UNIT") {
            float offset = hovered ? 5.0f : 0.0f;
            float radius = 15.0f + (hovered ? 3.0f : 0.0f);
            int font_size = hovered ? 20 : 15;

            Vector2 stat_pos = { current_bounds.x + 20.0f + offset, current_bounds.y + 20.0f + offset };

            // helper
            auto DrawStat = [&](int value, Color col, float y_off) {
                DrawCircleLines(stat_pos.x, stat_pos.y + y_off, radius, col);
                std::string txt = std::to_string(value);
                int spacing = MeasureText(txt.c_str(), font_size) / 2;
                DrawText(txt.c_str(), stat_pos.x - spacing, stat_pos.y + y_off - (font_size/2), font_size, col);
            };

            DrawStat(state.strength, GREEN, 0);
            DrawStat(state.armor,    BLUE,  30);
            DrawStat(state.attack,   RED,   60);
        }

        // name rendering
        int name_size = hovered ? 12 : 10;
        int textWidth = MeasureText(card_data->get_name().c_str(), name_size);
        float posX = current_bounds.x + (current_bounds.width / 2.0f) - (static_cast<float>(textWidth) / 2.0f);
        DrawText(card_data->get_name().c_str(), static_cast<int>(posX),
                 static_cast<int>(current_bounds.y) + static_cast<int>(current_bounds.height) - (hovered ? 30 : 25),
                 name_size, GREEN);
    }
}

void widget_card::update(float dt) {
    Vector2 mouse_pos = render_config::get_virtual_mouse();
    hovered = CheckCollisionPointRec(mouse_pos, base_bounds);

    if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        hover_timer += 0.01f;
        triggered = true;
    }
}
