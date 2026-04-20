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
    if (card_ctx.position == card_position::DECK) return;

    Rectangle draw_rect = current_bounds;

    // hover animation
    if (card_ctx.detail == card_detail::MATCH || card_ctx.detail == card_detail::MAX) {
        float lift = hover_timer * 15.0f;
        float expansion = hover_timer * 10.0f;
        draw_rect.y -= lift;
        draw_rect.x -= expansion / 2.0f;
        draw_rect.width += expansion;
        draw_rect.height += expansion;
    }

    switch (card_ctx.detail) {
        case card_detail::MIN: {
            DrawRectangleRec(draw_rect, Fade(GREEN, 0.1f));
            DrawRectangle(draw_rect.x, draw_rect.y, 5, draw_rect.height, GREEN);
            DrawText(card_data->get_name().c_str(), draw_rect.x + 15, draw_rect.y + 10, 20, GREEN);
            break;
        }
        case card_detail::MAX:
        case card_detail::MATCH: {
            draw_full_visual(draw_rect);
            break;
        }
    }
}

void widget_card::update(float dt) {

    Vector2 mouse_pos = render_config::get_virtual_mouse();

    // collision check
    hovered = CheckCollisionPointRec(mouse_pos, current_bounds);

    // hover animation
    float hover_speed = 10.0f;
    if (hovered) hover_timer += hover_speed * dt;
    else hover_timer -= hover_speed * dt;
    hover_timer = fmaxf(0.0f, fminf(hover_timer, 1.0f));

    // smooth lerping to new (potentially scrolled) position
    float lerp_speed = 12.0f;
    float t = 1.0f - expf(-lerp_speed * dt);

    current_bounds.x += (target_bounds.x - current_bounds.x) * t;
    current_bounds.y += (target_bounds.y - current_bounds.y) * t;
    current_bounds.width += (target_bounds.width - current_bounds.width) * t;
    current_bounds.height += (target_bounds.height - current_bounds.height) * t;

    // click detection
    if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        triggered = true;
    }
}

void widget_card::sync_card_context(const card_context& new_ctx) {
    this->card_ctx = new_ctx;
    this->target_bounds = new_ctx.card_bounds;
}

void widget_card::set_bounds(Rectangle new_bounds) {
    this->base_bounds = new_bounds;
    // if it's the first time setting bounds, snap the current bounds to it
    if (current_bounds.width == 0) {
        current_bounds = new_bounds;
        target_bounds = new_bounds;
    }
}

void widget_card::draw_at(Rectangle target_rect) const {
    switch (card_ctx.detail) {
        case card_detail::MIN: {
            DrawRectangleRec(target_rect, Fade(GREEN, 0.1f));
            DrawRectangle(target_rect.x, target_rect.y, 5, target_rect.height, GREEN);
            DrawText(card_data->get_name().c_str(), target_rect.x + 15, target_rect.y + 10, 20, GREEN);
            break;
        }

        case card_detail::MAX:
        case card_detail::MATCH: {
            draw_full_visual(target_rect);
            break;
        }
    }
}

void widget_card::draw_full_visual(Rectangle draw_rect) const {
    Color border_color = card_ctx.border_color;

    // stance logic
    if (card_ctx.detail == card_detail::MATCH) {
        if (card_ctx.position == card_position::HAND || card_ctx.position == card_position::ROW) {
            if (auto unit = dynamic_cast<const card_unit*>(card_data)) {
                switch (unit->get_stance()) {
                    case stances::AGGRESSIVE:  border_color = RED;      break;
                    case stances::SUPPRESSIVE: border_color = GREEN;    break;
                    case stances::DEFENSIVE:   border_color = BLUE;     break;
                }
            }
        }
    }

    // draw frame
    DrawRectangleLinesEx(draw_rect, 2.0f, border_color);

    // draw texture
    if (card_texture.id > 0) {
        DrawTexturePro(card_texture,
            { 0, 0, static_cast<float>(card_texture.width), static_cast<float>(card_texture.height) },
            draw_rect, { 0, 0 }, 0.0f, WHITE);
    } else {
        // fallback: dark placeholder
        DrawRectangleRec(draw_rect, {20, 20, 20, 255});
    }

    if (card_ctx.face_up) {
        // draw stats
        if (card_ctx.detail == card_detail::MATCH && card_data->get_card_type() == card_type::UNIT) {
            Vector2 stat_pos = {draw_rect.x, draw_rect.y + 10.0f};
            int strength = (card_ctx.position == card_position::ROW) ? card_ctx.virtual_strength : card_ctx.strength;

            Color stat_color = WHITE;
            if (card_ctx.position == card_position::ROW) {
                if (strength > card_ctx.strength) stat_color = GREEN;
                else if (strength < card_ctx.strength) stat_color = RED;
            }

            draw_stat_banner(stat_pos, 25.0f + (hover_timer * 5.0f), 20.0f, strength, WHITE, stat_color);
            stat_pos.y += 25.0f;
            draw_stat_banner(stat_pos, 25.0f + (hover_timer * 5.0f), 20.0f, card_ctx.armor, BLUE, WHITE);
            stat_pos.y += 25.0f;
            draw_stat_banner(stat_pos, 25.0f + (hover_timer * 5.0f), 20.0f, card_ctx.attack, RED, WHITE);
        }

        // draw name
        int name_size = (card_ctx.detail == card_detail::MAX) ? 12 : static_cast<int>(10.0f + (hover_timer * 2.0f));
        int textWidth = MeasureText(card_data->get_name().c_str(), name_size);
        DrawText(card_data->get_name().c_str(),
                 draw_rect.x + (draw_rect.width / 2.0f) - (textWidth / 2.0f),
                 draw_rect.y + draw_rect.height - (20.0f + (hover_timer * 5.0f)), name_size, GREEN);
    }
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
