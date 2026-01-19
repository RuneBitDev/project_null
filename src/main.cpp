#include "raylib.h"
#include <iostream>
#include "game/factory.h"
#include "game/player.h"
#include "game/engine/game_state.h"
#include "game/engine/menu_state.h"
#include "game/engine/state_manager.h"

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "PROJECT NULL");
    SetTargetFPS(60);

    state_manager manager;
    factory game_factory;
    renderer renderer;

    if (!game_factory.load_master_data("../data.sqlite")) {
        std::cerr << "CRITICAL ERROR: Could not load data.sqlite" << std::endl;
        CloseWindow();
        return 1;
    }

    player p1("Arasaka", game_factory.build_deck("arasaka"));
    player p2("Barghest", game_factory.build_deck("barghest"));

    manager.push_state(std::make_unique<menu_state>());

    while (!WindowShouldClose()) {

        manager.handle_input();

        float dt = GetFrameTime();
        manager.update(dt);

        BeginDrawing();
        manager.render(renderer);
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}