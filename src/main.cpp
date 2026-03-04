#include "raylib.h"
#include <iostream>
#include "core/factory.h"
#include "engine/menu_state.h"
#include "engine/state_manager.h"
#include "visual/render_config.h"
#include "visual/texture_factory.h"

int main() {

    render_config::Res current_res = render_config::screen_1440p;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(current_res.width, current_res.height, "PROJECT NULL");
    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(render_config::VIRTUAL_WIDTH, render_config::VIRTUAL_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    state_manager manager;
    factory game_factory;
    texture_factory texture_factory;
    renderer renderer;

    if (!game_factory.load_master_data("data.sqlite")) {
        std::cerr << "CRITICAL ERROR: Could not load data.sqlite" << std::endl;
        CloseWindow();
        return 1;
    } else {
        std::cout << "[INIT] Factory loaded " << game_factory.get_unit_library().size()
              << " units and " << game_factory.get_special_library().size()
              << " special cards." << std::endl;
    }
    if (!texture_factory.initialize_manifest("data.sqlite")) {
        std::cerr << "CRITICAL ERROR: Could not load data.sqlite" << std::endl;
        CloseWindow();
        return 1;
    }


    manager.push_state(std::make_unique<menu_state>(game_factory, texture_factory));

    while (!WindowShouldClose()) {

        manager.handle_input();
        float dt = GetFrameTime();
        manager.update(dt, renderer);

        if (IsKeyPressed(KEY_F1)) render_config::apply_resolution(render_config::screen_720p);
        if (IsKeyPressed(KEY_F2)) render_config::apply_resolution(render_config::screen_1080p);
        if (IsKeyPressed(KEY_F3)) render_config::apply_resolution(render_config::screen_1440p);
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();

        BeginTextureMode(target);
            manager.render(renderer);
        EndTextureMode();

        BeginDrawing();
            DrawTexturePro(target.texture,
                { 0, 0, static_cast<float>(target.texture.width), static_cast<float>(-target.texture.height) },
                { 0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) },
                { 0, 0}, 0.0f, WHITE);

            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}
