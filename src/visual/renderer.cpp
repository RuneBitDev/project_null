#include "visual/renderer.h"

#include <iostream>

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
    draw_board(ctx);
    draw_special_board(ctx);
    draw_hand(ctx.p1);
    draw_graveyard(ctx.p1, ctx.p2);

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

void renderer::draw_card(const std::unique_ptr<card>& card_ptr, float x, float y, bool is_face_up,
    const render_context* ctx, row_side side, row_type type) {

    ui_card card(x, y, is_face_up);
    ui_element::update_card(card);

    if (card.is_hovered) {
        float scale = 1.1f;
        float new_w = card.bounds.width * scale;
        float new_h = card.bounds.height * scale;

        card.bounds.x -= (new_w - card.bounds.width) / 2;
        card.bounds.y -= (new_h - card.bounds.height) / 2;
        card.bounds.width = new_w;
        card.bounds.height = new_h;

        DrawRectangleRec(card.bounds, BLACK);
    }

    DrawRectangleLinesEx(card.bounds, card.is_hovered ? 3 : 2, GREEN);

    // draw values
    if (is_face_up) {
        if (auto* unit = dynamic_cast<card_unit*>(card_ptr.get())) {
            float circle_radius = card.is_hovered ? 18.0f : 15.0f;
            float circle_x = card.bounds.x + (card.is_hovered ? 25.0f : 20.0f);
            float circle_y = card.bounds.y + (card.is_hovered ? 25.0f : 20.0f);

            DrawCircleLines(circle_x, circle_y, circle_radius, GREEN);
            DrawCircleLines(circle_x, circle_y + 30, circle_radius, BLUE);
            DrawCircleLines(circle_x, circle_y + 60, circle_radius, RED);
            int strength;
            int armor = unit->get_armor();
            int attack = unit->get_attack();
            if (ctx) {
                strength = unit->get_virtual_strength(ctx->b, side, type);
            } else {
                strength = unit->get_strength();
            }
            // draw number values
            std::string str_text = std::to_string(strength);
            std::string arm_text = std::to_string(armor);
            std::string att_text = std::to_string(attack);
            int font_size = card.is_hovered ? 20 : 15;
            DrawText(str_text.c_str(), circle_x - (font_size/3), circle_y - (font_size/2), font_size, GREEN);
            DrawText(arm_text.c_str(), circle_x - (font_size/3), circle_y - (font_size/2) +30, font_size, BLUE);
            DrawText(att_text.c_str(), circle_x - (font_size/3), circle_y - (font_size/2) +60, font_size, RED);
        }

        int name_size = card.is_hovered ? 12 : 10;
        draw_text_in_rect(card_ptr->get_name().c_str(), card.bounds, static_cast<int>(card.bounds.height) - (card.is_hovered ? 30 : 25), name_size, GREEN);
    }
}


void renderer::draw_hand(const player &player) {
    float x_offset = render_config::hand::X_OFFSET;

    for (const auto& card_ptr : player.get_hand()) {
        draw_card(card_ptr, x_offset, render_config::hand::Y_OFFSET, true, nullptr);
        x_offset += render_config::card::CARD_WIDTH + 10;
    }
}

void renderer::draw_graveyard(const player& p1, const player& p2) {
    float y_offset_p1 = render_config::graveyard::GY_Y + 550;
    float y_offset_p2 = render_config::graveyard::GY_Y - 550;
    float gy_x = render_config::graveyard::GY_X;
    Rectangle gy_bounds_p1 = {gy_x, y_offset_p1, 150, 200};
    Rectangle gy_bounds_p2 = {gy_x, y_offset_p2, 150, 200};
    int count = 0;

    DrawRectangleLinesEx(gy_bounds_p1, 10, DARKGREEN);
    for (const auto& card_ptr : p1.get_graveyard()) {
        draw_card(card_ptr, gy_x + 20 + (count * 2), y_offset_p1 + 25 - (count * 2), false, nullptr);
        count ++;
    }
    DrawRectangleLinesEx(gy_bounds_p2, 10, DARKGREEN);
    for (const auto& card_ptr : p2.get_graveyard()) {
        draw_card(card_ptr, gy_x + 20 + (count * 2), y_offset_p2 + 25 - (count * 2), false, nullptr);
        count ++;
    }

}

void renderer::draw_board(const render_context& ctx) {
    float start_x = render_config::board::START_X;
    float start_y_opponent = render_config::board::START_Y_OPPONENT;
    float start_y_player = render_config::board::START_Y_PLAYER;
    float row_spacing = render_config::card::CARD_HEIGHT + 30.0f;
    float split_width = (render_config::board::BOARD_WIDTH / 2.0f) - 5.0f;

    for (int side = 0; side < 2; side++) {
        row_side current_side = static_cast<row_side>(side);

        for (int type = 0; type < 4; type++) {
            row_type current_type = static_cast<row_type>(type);

            float row_y;
            float current_row_x = start_x;
            float current_row_width = render_config::board::BOARD_WIDTH;

            int visual_slot = (type == 3) ? 2 : type;

            if (side == 1) {
                row_y = start_y_opponent + ((2-visual_slot) * row_spacing);
            } else {
                row_y = start_y_player + (visual_slot * row_spacing);
            }

            if (type >= 2) {
                current_row_width = split_width;
                if (type == 3) {
                    current_row_x = start_x + split_width + 10.0f;
                }
            }

            DrawRectangleLines(current_row_x - 10, row_y -5, current_row_width, render_config::card::CARD_HEIGHT + 10, DARKGREEN);

            std::string type_label = ctx.b.get_row_name(current_type);
            DrawText(type_label.c_str(), current_row_x, row_y - 20, 15, DARKGREEN);

            const auto& row_cards = ctx.b.get_row_cards(side, type);
            float current_x = current_row_x;

            for (const auto& card_ptr : row_cards) {
                float card_spacing = 10.0f;
                draw_card(card_ptr, current_x, row_y, true, &ctx, current_side, current_type);
                current_x += render_config::card::CARD_WIDTH + card_spacing;
            }

            // row scores
            int row_score = ctx.b.calculate_row_score(static_cast<row_side>(side), static_cast<row_type>(type));
            std::string row_score_text = std::to_string(row_score);
            float row_score_x_offset = (type == 3) ? (current_row_x + current_row_width + 5) : (current_row_x - 60);
            DrawText(row_score_text.c_str(), row_score_x_offset, row_y + (render_config::card::CARD_HEIGHT/2) - 10, 20, DARKGREEN);

        }
        // side scores
        int side_score = ctx.b.calculate_total_score(static_cast<row_side>(side));
        std::string side_score_text = std::to_string(side_score);
        float side_score_y_offset = (side == 1) ? 400 : 800;
        DrawCircleLines(600, side_score_y_offset, 40, DARKGREEN);
        DrawText(side_score_text.c_str(), 580, side_score_y_offset - 20, 40, DARKGREEN);

    }
}

void renderer::draw_special_board(const render_context& ctx) {

    float special_x = render_config::board::START_X + render_config::board::BOARD_WIDTH + 20.0f;
    float center_y = (render_config::board::START_Y_OPPONENT + render_config::board::START_Y_PLAYER + render_config::card::CARD_HEIGHT) / 1.40f;

    for (int side = 0; side < 2; side++) {
        row_side current_side = static_cast<row_side>(side);
        float vertical_spacing = 40.0f;

        float row_y = (side == 1) ?
                          (center_y - render_config::card::CARD_HEIGHT - vertical_spacing / 2) :
                          (center_y + vertical_spacing / 2);

        DrawRectangleLines(special_x - 5, row_y - 5,
                           render_config::card::CARD_WIDTH + 10,
                           render_config::card::CARD_HEIGHT + 10, DARKGREEN);

        DrawText("SPECIAL", special_x, row_y - 20, 15, DARKGREEN);

        const auto& special_cards = ctx.b.get_row_cards(side, 4);
        float current_x = special_x;

        for (const auto& card_ptr : special_cards) {
            draw_card(card_ptr, current_x, row_y, true, &ctx, current_side, row_type::SPECIAL);
            current_x += 15.0f;;
        }
    }
}
