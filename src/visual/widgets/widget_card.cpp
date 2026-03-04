#include "visual/widgets/widget_card.h"
#include "core/components/card_unit.h"
#include "visual/render_config.h"
#include <cmath>
#include <iostream>

#include "visual/texture_factory.h"

widget_card::widget_card(const card* c_ptr, const card_context &card_ctx)
    : card_data(c_ptr), base_bounds(card_ctx.card_bounds), current_bounds(card_ctx.card_bounds), target_bounds(card_ctx.card_bounds), card_ctx(card_ctx) {
    if (texture_factory::instance) {
        this->card_texture = texture_factory::instance->get_texture_for_card(c_ptr->get_id());
    }
}

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


    float thickness = 2.0f;
    DrawRectangleLinesEx(draw_rect, thickness, border_color);

    if (card_ctx.face_up) {

        DrawTexturePro(card_texture,
            { 0, 0, static_cast<float>(card_texture.width), static_cast<float>(card_texture.height) },
            draw_rect,
            { 0, 0 }, 0.0f, WHITE);

        if (card_data->get_card_type() == "UNIT") {

            Vector2 stat_pos = {draw_rect.x,draw_rect.y + 10.0f};

            int strength = card_ctx.strength;
            auto stat_color = WHITE;
            float banner_w = 25.0f + (hover_timer * 5.0f);
            float banner_h = 20.0f;

            if (card_ctx.position == card_position::ROW) {
                strength = card_ctx.virtual_strength;
                if (strength > card_ctx.strength) stat_color = GREEN;
                else if (strength < card_ctx.strength) stat_color = RED;
            }

            draw_stat_banner(stat_pos, banner_w, banner_h, strength, WHITE, stat_color);
            stat_pos.y += 25.0f;
            draw_stat_banner(stat_pos, banner_w, banner_h, card_ctx.armor, BLUE, WHITE);
            stat_pos.y += 25.0f;
            draw_stat_banner(stat_pos, banner_w, banner_h, card_ctx.attack, RED, WHITE);
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


void widget_card::draw_stat_banner(Vector2 pos, float width, float height, int value, Color base_color, Color value_color) const{

    Vector2 vertices[6] = {
        { pos.x, pos.y },
        { pos.x + width, pos.y },
        { pos.x + width + (height/2), pos.y + height/2 },
        { pos.x + width, pos.y + height },
        { pos.x, pos.y + height },
        { pos.x + (height/2), pos.y + height/2 }
    };
    DrawTriangleFan(vertices, 6, base_color);

    Vector2 outline[7] = {
        vertices[0], vertices[1], vertices[2],
        vertices[3], vertices[4], vertices[5], vertices[0]
    };
    DrawLineStrip(outline, 7, base_color);

    int fontSize = static_cast<int>(height * 0.6f);
    std::string text = std::to_string(value);

    int textWidth = MeasureText(text.c_str(), fontSize);

    float textPosX = pos.x + (width / 2.0f) - (textWidth / 2.0f) + (height / 4.0f);
    float textPosY = pos.y + (height / 2.0f) - (fontSize / 2.0f);

    DrawText(text.c_str(), static_cast<int>(textPosX), static_cast<int>(textPosY), fontSize, value_color);
}
