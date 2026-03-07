#include "visual/ui_util.h"

#include "visual/render_config.h"

void ui::init() {
    header_font = LoadFontEx("data/fonts/SixtyfourConvergence-Regular.ttf", 64, nullptr, 0);
    console_font = LoadFontEx("data/fonts/ShareTechMono-Regular.ttf", 64, nullptr, 0);
}

void ui::unload() {
    UnloadFont(header_font);
    UnloadFont(console_font);
}

void ui::draw_text(const char *text, Vector2 pos, float size, Color col, bool centered) {
    Vector2 finalPos = pos;
    float spacing = 1.5f;
    if (centered) {
        Vector2 textSize = MeasureTextEx(console_font, text, size, spacing);
        finalPos.x -= textSize.x / 2.0f;
    }
    // drop shadow for readability
    DrawTextEx(console_font, text, { finalPos.x + 1, finalPos.y + 1 }, size, spacing, BLACK);
    // main text
    DrawTextEx(console_font, text, finalPos, size, spacing, col);
}

void ui::draw_text_header(const char *text, int y, float size, Color col) {

    Vector2 textSize = MeasureTextEx(header_font, text, size, 2.0f);
    float x_pos = (render_config::VIRTUAL_WIDTH - textSize.x) / 2.0f;
    float spacing = 1.5f;

    float jitterX = static_cast<float>(GetRandomValue(-100, 100)) / 100.0f;
    float jitterY = static_cast<float>(GetRandomValue(-100, 100)) / 100.0f;

    DrawTextEx(header_font, text, { x_pos - 2 + jitterX, static_cast<float>(y) + jitterY }, size, spacing, Fade(RED, 0.4f));
    DrawTextEx(header_font, text, { x_pos + 2 - jitterX, static_cast<float>(y)- jitterY }, size, spacing, Fade(BLUE, 0.4f));
    DrawTextEx(header_font, text, {x_pos, static_cast<float>(y)}, size, spacing, col);
}
