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

void renderer::draw_card(const std::unique_ptr<card>& card, float x, float y, bool is_reverse) {
    Rectangle rect = {x, y , render_config::card::CARD_WIDTH, render_config::card::CARD_HEIGHT};
    DrawRectangleLinesEx(rect, 2 , GREEN);

    if (!is_reverse) {
        if (auto* unit = dynamic_cast<card_unit*>(card.get())) {
            // This block only runs if the card is actually a unit
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
    float x_offset = 100.0f;
    for (auto& card_ptr : player.get_hand()) {
        draw_card(card_ptr, x_offset, 1100, false);
        x_offset += render_config::card::CARD_WIDTH + 10;
    }
}

void renderer::draw_graveyard(const player &player) {
    float gy_x = 1100.0f;
    float gy_y = 600.0f;
    int count = 0;
    for (const auto& card_ptr : player.get_graveyard()) {
        draw_card(card_ptr, gy_x + (count * 2), gy_y + (count * 2), true);
        count ++;
    }
}

void renderer::draw_board(const board &board) {
    float start_x = 200.0f;
    float start_y_opponent = 100.0f;
    float start_y_paler = 600.0f;

    float row_spacing = render_config::card::CARD_HEIGHT + 20.0f;
    float card_spacing = 10.0f;

    for (int side = 0; side < 2; side++) {
        for (int type = 0; type < 4; type++) {
            float row_y;
            if (side == 1) {
                row_y = start_y_opponent + (type * row_spacing);
            } else {
                row_y = start_y_paler + ((3 - type) * row_spacing);
            }

            DrawRectangleLines(start_x - 10, row_y -5, 1000, render_config::card::CARD_HEIGHT + 10, DARKGREEN);

            const auto& row_cards = board.get_row_cards(side, type);

            float current_x = start_x;
            for (const auto& card_ptr : row_cards) {
                draw_card(card_ptr, current_x, row_y, false);
                current_x += render_config::card::CARD_WIDTH + card_spacing;
            }

            int score = board.calculate_row_score(static_cast<row_side>(side), static_cast<row_type>(type));
            std::string score_text = std::to_string(score);
            DrawText(score_text.c_str(), start_x - 60, row_y + (render_config::card::CARD_HEIGHT/2) - 10, 20, DARKGREEN);

        }
    }
}
