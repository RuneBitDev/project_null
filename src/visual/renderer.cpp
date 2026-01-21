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
    draw_button_rec(render_config::ui::START_BUTTON, "START GAME");
}

void renderer::draw_game(const board &b, const player &p1, player &p2) {
    ClearBackground(BLACK);

    draw_hand(p1);
    draw_graveyard(p1);
    draw_board(b);
    // draw graveyard

}


// ---------------------------------------------------------------------
// HELPER FUNCTIONS
// ---------------------------------------------------------------------


void renderer::draw_button_rec(Rectangle rec, const char *text) {
    bool hovered = CheckCollisionPointRec(render_config::get_virtual_mouse(), rec);
    Color tint = hovered ? LIME : BLACK;

    DrawRectangleRec(rec, tint);
    DrawRectangleLinesEx(rec, 2 , GREEN);

    int fontSize = 40;
    int textWidth = MeasureText(text, fontSize);
    DrawText(text,
        rec.x + (rec.width / 2 - textWidth / 2),
        rec.y + (rec.height / 2 - fontSize / 2),
        fontSize, GREEN);
}

void renderer::draw_text_centered(const char* text, int y, int size, Color color) {
    int width = MeasureText(text, size);
    DrawText(text, render_config::VIRTUAL_WIDTH / 2 - width / 2, y, size, color);
}

void renderer::draw_text_in_rect(const char* text, Rectangle rect, int y_offset, int size, Color color) {
    int textWidth = MeasureText(text, size);
    float posX = rect.x + (rect.width / 2.0f) - (textWidth / 2.0f);

    DrawText(text, (int)posX, (int)rect.y + y_offset, size, color);
}

void renderer::draw_card(const std::unique_ptr<card>& card, float x, float y, bool is_reverse) {
    Rectangle rect = {x, y , render_config::card::CARD_WIDTH, render_config::card::CARD_HEIGHT};
    DrawRectangleLinesEx(rect, 2 , GREEN);

    if (!is_reverse) {
        if (auto* unit = dynamic_cast<card_unit*>(card.get())) {

            float circle_x = x + 20;
            float circle_y = y + 20;
            DrawCircleLines(circle_x, circle_y, 15, GREEN);

            std::string str_text = std::to_string(unit->get_strength());
            DrawText(str_text.c_str(), circle_x - 5, circle_y - 7, 15, GREEN);
        }

        std::string name = card->get_name();
        int name_size = 10;
        int name_width = MeasureText(name.c_str(), name_size);

        float name_x = x + (render_config::card::CARD_WIDTH / 2) - (name_width / 2);
        float name_y = y + render_config::card::CARD_HEIGHT - 25;

        DrawText(name.c_str(), (int)name_x, (int)name_y, name_size, GREEN);
    }
}

void renderer::draw_hand(const player &player) {
    float x_offset = render_config::hand::X_OFFSET;
    Vector2 mouse_pos = render_config::get_virtual_mouse();

    const auto& hand = player.get_hand();

    for (size_t i = 0; i < hand.size(); ++i) {
        float x = x_offset;
        float y = render_config::hand::Y_OFFSET;
        float width = render_config::card::CARD_WIDTH;
        float height = render_config::card::CARD_HEIGHT;

        Rectangle card_rect = { x, y, width, height };
        bool is_hovered = CheckCollisionPointRec(mouse_pos, card_rect);

        if (is_hovered) {
            float scale = 1.1f;
            float new_w = width * scale;
            float new_h = height * scale;

            float diff_w = new_w - width;
            float diff_h = new_h - height;

            draw_card_scaled(hand[i], x - diff_w/2, y - diff_h/2, new_w, new_h);
        } else {
            draw_card(hand[i], x, y, false);
        }

        x_offset += width + 10;
    }
}

void renderer::draw_card_scaled(const std::unique_ptr<card>& card, float x, float y, float w, float h) {
    Rectangle rect = { x, y, w, h };
    DrawRectangle(x, y, w, h, BLACK);
    DrawRectangleLinesEx(rect, 3, GREEN);

    if (auto* unit = dynamic_cast<card_unit*>(card.get())) {
        DrawCircleLines(x + 25, y + 25, 18, GREEN);
        std::string str_text = std::to_string(unit->get_strength());
        DrawText(str_text.c_str(), x + 18, y + 15, 20, GREEN);
    }

    std::string name = card->get_name();
    int fontSize = 11;
    int relative_y_offset = (int)h - 30;

    draw_text_in_rect(name.c_str(), rect, relative_y_offset, fontSize, GREEN);
}

void renderer::draw_graveyard(const player &player) {

    int count = 0;
    for (const auto& card_ptr : player.get_graveyard()) {
        draw_card(card_ptr, render_config::graveyard::GY_X + (count * 2), render_config::graveyard::GY_Y + (count * 2), true);
        count ++;
    }
}

void renderer::draw_board(const board &board) {
    float start_x = render_config::board::START_X;
    float start_y_opponent = render_config::board::START_Y_OPPONENT;
    float start_y_player = render_config::board::START_Y_PLAYER;

    float row_spacing = render_config::card::CARD_HEIGHT + 20.0f;
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
            DrawText(type_label.c_str(), current_row_x, row_y - 15, 15, DARKGREEN);

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
