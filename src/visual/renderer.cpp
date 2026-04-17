#include "visual/renderer.h"
#include "visual/render_config.h"
#include "visual/ui_util.h"
#include <cmath>



renderer::renderer() {
    ui::init();
}

renderer::~renderer() {
    ui::unload();
}

void renderer::draw_background() {
    ClearBackground({ 10, 10, 15, 255 });
    for (int i = 0; i < render_config::VIRTUAL_HEIGHT; i += 4) {
        DrawLine(0, i, render_config::VIRTUAL_WIDTH, i, Fade(RAYWHITE, 0.03f));
    }
}

void renderer::draw_start_screen() {
    ClearBackground({ 10, 10, 15, 255 });
    for (int i = 0; i < render_config::VIRTUAL_HEIGHT; i += 4) {
        DrawLine(0, i, render_config::VIRTUAL_WIDTH, i, Fade(RAYWHITE, 0.03f));
    }

    ui::draw_text_header("PROJECT NULL", 300, 80, GREEN);
    ui::draw_text_header("SYSTEM INITIALIZED", 450, 30, Fade(GREEN, 0.6f));

    float alpha = (sinf(GetTime() * 4.0f) * 0.5f + 0.5f);
    ui::draw_text_header(">> PRESS ENTER <<", 900, 40, Fade(GREEN, alpha));
}

void renderer::draw_construction() {
    ClearBackground({ 10, 10, 15, 255 });
    for (int i = 0; i < render_config::VIRTUAL_HEIGHT; i += 4) {
        DrawLine(0, i, render_config::VIRTUAL_WIDTH, i, Fade(RAYWHITE, 0.03f));
    }

    float width = render_config::VIRTUAL_WIDTH;
    float height = render_config::VIRTUAL_HEIGHT;

    // central divider
    Rectangle bridge = { width * 0.62f, 0, width * 0.08f, height };
    DrawRectangleRec(bridge, { 15, 15, 20, 200 });
    DrawLineEx({ bridge.x, 0 }, { bridge.x, height }, 2.0f, GREEN);

    // header
    ui::draw_text_header("DECK_CONSTRUCT_MODE // V.04", 50, 40, GREEN);

    // resource thingy
    float ram_usage = 0.75f;
    DrawRectangle(width * 0.75f, 50, 400, 20, Fade(GREEN, 0.2f));
    DrawRectangle(width * 0.75f, 50, 400 * ram_usage, 20, GREEN);
    DrawText("RAM USAGE", width * 0.75f, 30, 15, GREEN);


    // deck grid
    for (int i = 0; i < 10; i++) {
        float y_pos = 150 + (i * 55);
        Rectangle slot = { width * 0.72f, y_pos, 500, 50 };

        DrawRectangleRec(slot, Fade(GREEN, 0.1f));
        DrawRectangle(slot.x, slot.y, 5, slot.height, GREEN);

        DrawText("UNIT_NAME_SHARD", slot.x + 15, slot.y + 15, 20, GREEN);
        DrawText("10", slot.x + 460, slot.y + 15, 20, GREEN); // Cost
    }

}


void renderer::draw_menu(int p1_idx, int p2_idx, const std::vector<std::string>& factions) {
    ClearBackground({ 10, 10, 15, 255 });
    for (int i = 0; i < 1440; i += 5) {
        DrawLine(0, i, 2560, i, Fade(RAYWHITE, 0.05f));
    }

    float centerX = render_config::VIRTUAL_WIDTH / 2.0f;

    auto draw_selection_slot = [&](int idx, float y, Color theme, const char* title) {
        Texture2D tex = texture_factory::instance->get_texture("selection_" + factions[idx]);
        Rectangle dest = { centerX - 600, y, 1200, 400 };


        if (tex.id > 0) {
            DrawTexturePro(tex, {0, 0, static_cast<float>(tex.width), static_cast<float>(tex.height)}, dest, {0,0}, 0.0f, WHITE);
        }

        // cyberpunk frame
        DrawRectangleLinesEx(dest, 2.0f, Fade(theme, 0.5f));
        DrawRectangle(dest.x, dest.y - 40, 300, 40, theme);
        DrawText(title, dest.x + 10, dest.y - 35, 25, BLACK);

        std::string name = factions[idx];
        for(auto & c: name) c = toupper(c);
        DrawText(name.c_str(), dest.x + 10, dest.y + dest.height + 10, 40, theme);
    };

    draw_selection_slot(p2_idx, 180.0f, RED, "HOSTILE_ID");
    draw_selection_slot(p1_idx, 860.0f, GREEN, "OPERATIVE_USER");

    // central divider
    DrawLineEx({0, 720}, {centerX - 300, 720}, 2.0f, Fade(GREEN, 0.3f));
    DrawLineEx({centerX + 300, 720}, {2560, 720}, 2.0f, Fade(RED, 0.3f));


}