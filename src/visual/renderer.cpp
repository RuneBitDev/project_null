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
    // get the unit
    auto* unit = dynamic_cast<card_unit*>(card_ptr.get());

    // different color for stances, later probably a glow of sorts
    auto border_color = WHITE;
    if (unit) {
        switch (unit->get_stance()) {
            case stances::AGGRESSIVE:  border_color = RED;   break;
            case stances::SUPPRESSIVE: border_color = BLUE;  break;
            case stances::DEFENSIVE:   border_color = GREEN; break;
        }
    }
    DrawRectangleLinesEx(card.bounds, card.is_hovered ? 3 : 2, border_color);

    // draw values
    if (is_face_up) {
        if (unit) {
            // draw circles
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
            // draw numbers
            std::string str_text = std::to_string(strength);
            std::string arm_text = std::to_string(armor);
            std::string att_text = std::to_string(attack);
            int font_size = card.is_hovered ? 20 : 15;
            DrawText(str_text.c_str(), circle_x - (font_size/3), circle_y - (font_size/2), font_size, GREEN);
            DrawText(arm_text.c_str(), circle_x - (font_size/3), circle_y - (font_size/2) +30, font_size, BLUE);
            DrawText(att_text.c_str(), circle_x - (font_size/3), circle_y - (font_size/2) +60, font_size, RED);
        }

        // draw name
        int name_size = card.is_hovered ? 12 : 10;
        draw_text_in_rect(card_ptr->get_name().c_str(), card.bounds, static_cast<int>(card.bounds.height) - (card.is_hovered ? 30 : 25), name_size, GREEN);

    }


}


void renderer::draw_hand(const player &player) {
    const auto& hand = player.get_hand();
    int total_cards = static_cast<int>(hand.size());

    for (int i = 0; i < total_cards; i++) {
        Rectangle bounds = layout_manager::get_hand_card_bounds(i, total_cards);
        draw_card(hand[i], bounds.x, bounds.y, true, nullptr);
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
    for (int side_idx = 0; side_idx < 2; side_idx++) {
        row_side current_side = static_cast<row_side>(side_idx);

        for (int type_idx = 0; type_idx < 4; type_idx++) {
            row_type current_type = static_cast<row_type>(type_idx);
            const auto& row_cards = ctx.b.get_row_cards(side_idx, type_idx);
            int total_cards = static_cast<int>(row_cards.size());

            // --- ROW VISUALS ---
            card_location row_ref_loc { current_side, current_type, 0 };
            Rectangle row_ref = layout_manager::get_card_bounds(row_ref_loc, 1);
            float row_width = render_config::board::BOARD_WIDTH;
            float row_x = render_config::board::START_X;
            if (type_idx >= 2) {
                row_width = (render_config::board::BOARD_WIDTH / 2.0f) - 5.0f;
                if (type_idx == 3) row_x += row_width + 10.0f;
            }
            DrawRectangleLines(row_x - 10, row_ref.y - 5, row_width, render_config::card::CARD_HEIGHT + 10, DARKGREEN);

            // --- DRAW LABELS ---
            std::string type_label = ctx.b.get_row_name(current_type);
            DrawText(type_label.c_str(), row_x, row_ref.y - 20, 15, DARKGREEN);

            // --- DRAW CARDS ---
            for (int i = 0; i < total_cards; i++) {
                card_location loc { current_side, current_type, i };
                Rectangle card_rect = layout_manager::get_card_bounds(loc, total_cards);
                draw_card(row_cards[i], card_rect.x, card_rect.y, true, &ctx, current_side, current_type);
            }

            // --- ROW SCORE ---
            int row_score = ctx.b.calculate_row_score(current_side, current_type);
            float score_x = (type_idx == 3) ? (row_x + row_width + 10) : (row_x - 60);
            DrawText(std::to_string(row_score).c_str(), score_x, row_ref.y + (row_ref.height/2) - 10, 20, DARKGREEN);
        }

        // --- TOTAL SIDE SCORE ---
        int side_score = ctx.b.calculate_total_score(current_side);
        float side_score_y = (current_side == row_side::OPPONENT) ? 150 : 850;
        DrawCircleLines(600, side_score_y, 40, DARKGREEN);
        DrawText(std::to_string(side_score).c_str(), 580, side_score_y - 20, 40, DARKGREEN);
    }
}

void renderer::draw_special_board(const render_context& ctx) {

    float special_x = render_config::board::START_X + render_config::board::BOARD_WIDTH + 20.0f;
    float center_y = (render_config::board::START_Y + render_config::board::START_Y + render_config::card::CARD_HEIGHT) / 1.40f;

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
