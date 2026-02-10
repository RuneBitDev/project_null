#ifndef PROJECT_NULL_RENDERER_H
#define PROJECT_NULL_RENDERER_H
#include "raylib.h"
#include "ui_types.h"
#include "layout_manager.h"
#include "../core/components/board.h"
#include "../core/components/player.h"

struct render_context {
    const board& b;
    const player& p1;
    const player& p2;
};

class renderer {
public:
    void draw_start_screen();
    void draw_menu();
    void draw_game(const render_context& ctx);

private:
    void draw_button(button& btn);
    void draw_text_centered(const char* text, int y, int size, Color color);
    void draw_text_in_rect(const char* text, Rectangle rect, int y_offset, int size, Color color);
    void draw_card(const std::unique_ptr<card>& card_ptr, float x, float y, bool is_face_up,
        const render_context* ctx, row_side side = row_side::PLAYER, row_type type = row_type::MELEE);
    void draw_hand(const player& player);
    void draw_graveyard(const player& p1, const player& p2);
    void draw_board(const render_context& ctx);
    void draw_special_board(const render_context& ctx);
};

#endif //PROJECT_NULL_RENDERER_H