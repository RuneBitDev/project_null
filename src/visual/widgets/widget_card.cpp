#include "visual/widgets/widget_card.h"
#include "core/components/card_unit.h"
#include "visual/render_config.h"
#include <cmath>
#include <iostream>

widget_card::widget_card(const card* c_ptr, const card_context &card_ctx)
    : card_data(c_ptr), base_bounds(card_ctx.card_bounds), current_bounds(card_ctx.card_bounds), target_bounds(card_ctx.card_bounds), card_ctx(card_ctx) {}

void widget_card::draw() const {

    Color border_color = card_ctx.border_color;
    Rectangle draw_rect = current_bounds;

    float lift = hover_timer * 15.0f;
    float expansion = hover_timer * 10.0f;

    draw_rect.y -= lift;
    draw_rect.x -= expansion / 2.0f;
    draw_rect.width += expansion;
    draw_rect.height += expansion;

    if (card_ctx.position == card_position::HAND || card_ctx.position == card_position::ROW) {
        if (auto unit = dynamic_cast<const card_unit*>(card_data)) {
            switch (unit->get_stance()) {
                case stances::AGGRESSIVE:  border_color = RED;      break;
                case stances::SUPPRESSIVE: border_color = GREEN;    break;
                case stances::DEFENSIVE:   border_color = BLUE;     break;
            }
        }
    }


    float thickness = 2.0f + (hover_timer * 2.0f);
    DrawRectangleLinesEx(draw_rect, thickness, border_color);

    if (card_ctx.face_up) {

        if (card_data->get_card_type() == "UNIT") {
            float radius = 15.0f + (hover_timer * 3.0f);
            int font_size = static_cast<int>(15.0f + (hover_timer * 5.0f));

            Vector2 stat_pos = {
                draw_rect.x + 20.0f,
                draw_rect.y + 20.0f
            };

            // helper
            auto DrawStat = [&](int value, Color col, float y_off) {
                DrawCircleLines(stat_pos.x, stat_pos.y + y_off, radius, col);
                std::string txt = std::to_string(value);
                int spacing = MeasureText(txt.c_str(), font_size) / 2;
                DrawText(txt.c_str(), stat_pos.x - spacing, stat_pos.y + y_off - (font_size/2), font_size, col);
            };

            int strength = card_ctx.strength;
            Color stat_color = WHITE;

            if (card_ctx.position == card_position::ROW) {
                strength = card_ctx.virtual_strength;
                if (strength > card_ctx.strength) stat_color = GREEN;
                else if (strength < card_ctx.strength) stat_color = RED;
            }

            DrawStat(strength, stat_color, 0);

            DrawStat(card_ctx.armor,    BLUE,  30);
            DrawStat(card_ctx.attack,   RED,   60);
        }

        // name rendering
        int name_size = static_cast<int>(10.0f + (hover_timer * 2.0f));
        int textWidth = MeasureText(card_data->get_name().c_str(), name_size);
        float posX = draw_rect.x + (draw_rect.width / 2.0f) - (textWidth / 2.0f);
        float posY = draw_rect.y + draw_rect.height - (25.0f + (hover_timer * 5.0f));

        DrawText(card_data->get_name().c_str(), static_cast<int>(posX), static_cast<int>(posY), name_size, GREEN);
    }
}

void widget_card::update(float dt) {
    Vector2 mouse_pos = render_config::get_virtual_mouse();
    hovered = CheckCollisionPointRec(mouse_pos, current_bounds);

    // hover logic
    float hover_speed = 10.0f;
    if (hovered) {
        hover_timer += hover_speed * dt;
    } else {
        hover_timer -= hover_speed * dt;
    }
    hover_timer = std::fmaxf(0.0f, std::fminf(hover_timer, 1.0f)); // clamp

    // position lerping
    float lerp_speed = 9.0f;
    float t = 1.0f - expf(-lerp_speed * dt);
    current_bounds.x += (target_bounds.x - current_bounds.x) * t;
    current_bounds.y += (target_bounds.y - current_bounds.y) * t;
    current_bounds.width += (target_bounds.width - current_bounds.width) * t;
    current_bounds.height += (target_bounds.height - current_bounds.height) * t;

    // trigger logic
    if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        triggered = true;
    }


}

void widget_card::sync_card_context(const card_context& new_ctx) {
    this->card_ctx = new_ctx;
    this->target_bounds = new_ctx.card_bounds;
}

void widget_card::set_bounds(Rectangle new_bounds) {
    base_bounds = new_bounds;
}
