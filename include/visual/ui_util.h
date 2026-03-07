#ifndef PROJECT_NULL_UI_UTIL_H
#define PROJECT_NULL_UI_UTIL_H
#include "raylib.h"

namespace ui {
    inline Font console_font;
    inline Font header_font;

    void init();
    void unload();

    void draw_text(const char* text, Vector2 pos, float size, Color col, bool centered = false);
    void draw_text_header(const char* text, int y, float size, Color col);

    // maybe add like draw_panel or smth
}

#endif //PROJECT_NULL_UI_UTIL_H