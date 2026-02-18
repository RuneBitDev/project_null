#ifndef PROJECT_NULL_RENDERER_H
#define PROJECT_NULL_RENDERER_H
#include "raylib.h"
#include "ui_types.h"
#include "layout_manager.h"
#include "../core/components/board.h"
#include "../core/components/player.h"
#include "widgets/widget_board.h"
#include "widgets/widget_hand.h"

struct render_context {
    const board& b;
    const player& p1;
    const player& p2;
};

class renderer {
public:
    renderer();
    ~renderer();

    void draw_start_screen();
    void draw_menu();
    void draw_game(const render_context& ctx);

private:
    widget_board board_view;
    widget_hand hand_view;

    Shader matrixShader;
    int uTimeLoc;
    int uResLoc;

    void draw_button(button& btn);
    void draw_text_centered(const char* text, int y, int size, Color color);
    void draw_text_in_rect(const char* text, Rectangle rect, int y_offset, int size, Color color);
    void apply_matrix_rain();
};

#endif //PROJECT_NULL_RENDERER_H