#ifndef PROJECT_NULL_RENDERER_H
#define PROJECT_NULL_RENDERER_H
#include "raylib.h"
#include "ui_types.h"
#include "../game/components/board.h"
#include "../game/components/player.h"


class renderer {
public:
    void draw_start_screen();
    void draw_menu();
    void draw_game( const board& b, const player& p1, player& p2);

private:
    void draw_button(const button& btn);
    void draw_text_centered(const char* text, int y, int size, Color color);
    void draw_text_in_rect(const char* text, Rectangle rect, int y_offset, int size, Color color);
    void draw_card(const std::unique_ptr<card>& card_ptr, float x, float y, bool is_face_up);
    void draw_hand(const player& player);
    void draw_graveyard(const player& player);
    void draw_board(const board& board);
    void draw_special_board(const board& board);
};

#endif //PROJECT_NULL_RENDERER_H