#include "visual/renderer.h"
#include "visual/render_config.h"


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

void renderer::draw_game(const board &b, const player &p1, player &p2) {
    ClearBackground(BLACK);
    draw_button(render_config::ui::PASS_BUTTON);
    draw_hand(p1);
    draw_graveyard(p1);
    draw_board(b);
    draw_special_board(b);

}


// ---------------------------------------------------------------------
// HELPER FUNCTIONS
// ---------------------------------------------------------------------


void renderer::draw_button(const button& btn) {
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
        // Progression bar logic
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

void renderer::draw_card(const std::unique_ptr<card>& card_ptr, ui_card& card) {
    Rectangle rect = {card.bounds.x, card.bounds.y, card.bounds.width, card.bounds.height};


    if (card.is_hovered) {
        float scale = 1.1f;
        float new_w = rect.width * scale;
        float new_h = rect.height * scale;

        rect.x -= (new_w - rect.width) / 2;
        rect.y -= (new_h - rect.height) / 2;
        rect.width = new_w;
        rect.height = new_h;

        DrawRectangleRec(rect, BLACK);
    }

    DrawRectangleLinesEx(rect, card.is_hovered ? 3 : 2, GREEN);

    if (card.is_face_up) {
        if (auto* unit = dynamic_cast<card_unit*>(card_ptr.get())) {
            float circle_radius = card.is_hovered ? 18.0f : 15.0f;
            float circle_x = rect.x + (card.is_hovered ? 25 : 20);
            float circle_y = rect.y + (card.is_hovered ? 25 : 20);

            DrawCircleLines(circle_x, circle_y, circle_radius, GREEN);

            std::string str_text = std::to_string(unit->get_strength());
            int font_size = card.is_hovered ? 20 : 15;
            DrawText(str_text.c_str(), circle_x - (font_size/3), circle_y - (font_size/2), font_size, GREEN);
        }

        int name_size = card.is_hovered ? 12 : 10;
        draw_text_in_rect(card_ptr->get_name().c_str(), rect, static_cast<int>(rect.height) - (card.is_hovered ? 30 : 25), name_size, GREEN);
    }
}


void renderer::draw_hand(const player &player) {
    float x_offset = render_config::hand::X_OFFSET;

    for (const auto& card_ptr : player.get_hand()) {
        ui_card card = {x_offset, render_config::hand::Y_OFFSET, render_config::card::CARD_WIDTH, render_config::card::CARD_HEIGHT,
            false, false, true};
        draw_card(card_ptr, card);
        x_offset += render_config::card::CARD_WIDTH + 10;
    }
}

void renderer::draw_graveyard(const player &player) {

    int count = 0;
    for (const auto& card_ptr : player.get_graveyard()) {
        ui_card card = {
            render_config::graveyard::GY_X + (count * 2), render_config::graveyard::GY_Y + (count * 2),
            render_config::card::CARD_WIDTH, render_config::card::CARD_HEIGHT,
            false, false, false
        };
        draw_card(card_ptr, card);
        count ++;
    }
}

void renderer::draw_board(const board &board) {
    float start_x = render_config::board::START_X;
    float start_y_opponent = render_config::board::START_Y_OPPONENT;
    float start_y_player = render_config::board::START_Y_PLAYER;

    float row_spacing = render_config::card::CARD_HEIGHT + 30.0f;
    float card_spacing = 10.0f;

    float split_width = (render_config::board::BOARD_WIDTH / 2.0f) - 5.0f;

    for (int side = 0; side < 2; side++) {
        for (int type = 0; type < 4; type++) {
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

            std::string type_label = board.get_row_name(static_cast<row_type>(type));
            DrawText(type_label.c_str(), current_row_x, row_y - 20, 15, DARKGREEN);

            const auto& row_cards = board.get_row_cards(side, type);
            float current_x = current_row_x;

            for (const auto& card_ptr : row_cards) {
                draw_card(card_ptr, current_x, row_y, false);
                current_x += render_config::card::CARD_WIDTH + card_spacing;
            }

            int score = board.calculate_row_score(static_cast<row_side>(side), static_cast<row_type>(type));
            std::string score_text = std::to_string(score);

            float score_x_offset = (type == 3) ? (current_row_x + current_row_width + 5) : (current_row_x - 60);
            DrawText(score_text.c_str(), score_x_offset, row_y + (render_config::card::CARD_HEIGHT/2) - 10, 20, DARKGREEN);
        }
    }
}

void renderer::draw_special_board(const board &board) {

    float special_x = render_config::board::START_X + render_config::board::BOARD_WIDTH + 20.0f;
    float center_y = (render_config::board::START_Y_OPPONENT + render_config::board::START_Y_PLAYER + render_config::card::CARD_HEIGHT) / 1.40f;


    float vertical_spacing = 40.0f;

    for (int side = 0; side < 2; side++) {

        float row_y = (side == 1) ?
                      (center_y - render_config::card::CARD_HEIGHT - vertical_spacing / 2) :
                      (center_y + vertical_spacing / 2);

        DrawRectangleLines(special_x - 5, row_y - 5,
                           render_config::card::CARD_WIDTH + 10,
                           render_config::card::CARD_HEIGHT + 10, DARKGREEN);

        DrawText("SPECIAL", special_x, row_y - 20, 15, DARKGREEN);

        const auto& special_cards = board.get_row_cards(side, 4);
        float current_x = special_x;

        for (const auto& card_ptr : special_cards) {
            draw_card(card_ptr, current_x, row_y, false);
            current_x += 15.0f;
        }
    }
}
