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

    // draw hand
    float x_offset = 100.0f;
    for (auto& card_ptr : p1.get_hand()) {
        draw_card(card_ptr, x_offset, 1100, false);
        x_offset += render_config::card::CARD_WIDTH + 10;
    }

    // draw graveyard
    float gy_x = 1100.0f;
    float gy_y = 600.0f;
    int count = 0;
    for (const auto& card_ptr : p1.get_graveyard()) {
        draw_card(card_ptr, gy_x + (count * 2), gy_y + (count * 2), true);
        count ++;
    }
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
        int name_size = 24;
        int name_width = MeasureText(name.c_str(), name_size);

        float name_x = x + (render_config::card::CARD_WIDTH / 2) - (name_width / 2);
        float name_y = y + render_config::card::CARD_HEIGHT - 25;

        DrawText(name.c_str(), (int)name_x, (int)name_y, name_size, GREEN);
    }
}
