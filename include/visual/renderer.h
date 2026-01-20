#ifndef PROJECT_NULL_RENDERER_H
#define PROJECT_NULL_RENDERER_H
#include "raylib.h"
#include "game/board.h"
#include "game/player.h"


class renderer {
public:
    void draw_start_screen();
    void draw_menu();
    void draw_game( const board& b, const player& p1, player& p2);

private:
    void draw_button_rec(Rectangle rec, const char* text);
    void draw_text_centered(const char* text, int y, int size, Color color);
    void draw_card(const std::unique_ptr<card>& card, float x, float y, bool is_reverse);
};

#endif //PROJECT_NULL_RENDERER_H