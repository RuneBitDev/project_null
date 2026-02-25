#ifndef PROJECT_NULL_RENDER_CONFIG_H
#define PROJECT_NULL_RENDER_CONFIG_H
#include "raylib.h"
#include "visual/ui_types.h"

namespace render_config {

    constexpr int VIRTUAL_WIDTH = 2560;
    constexpr int VIRTUAL_HEIGHT = 1440;

    struct Res {
        int width;
        int height;
        const char *name;
    };

    constexpr Res screen_720p = {1280, 720, "1280x720"};
    constexpr Res screen_1080p = {1920, 1080, "1920x1080"};
    constexpr Res screen_1440p = {2560, 1440, "2560x1440"};

    namespace ui {
        inline button START_BUTTON = { 1080, 600, 400, 100, CLICKABLE, 0, "Start Game"};
        inline button PASS_BUTTON = { 100, 100, 300, 100, HOLDABLE, KEY_SPACE, "PASS"};
    }

    namespace card {
        constexpr float CARD_WIDTH = 100.0f;
        constexpr float CARD_HEIGHT = 150.0f;
        constexpr float CARD_SPACING = 10.0f;
    }

    namespace hand {
        constexpr float X_OFFSET = 700.0f;
        constexpr float Y_OFFSET = 1200.0f;
    }

    namespace graveyard {
        constexpr float GY_X = 2200.0f;
        constexpr float GY_Y = 600.0f;
        constexpr float GY_SPACING = 3.0f;
    }

    namespace deck {
        constexpr float DECK_X = 2200.0f;
        constexpr Rectangle DECK_PLAYER = {2350.0f, 980.0f, 130.0f, 180.0f};
        constexpr Rectangle DECK_OPPONENT = {2350.0f, 50.0f, 130.0f, 180.0f};
    }

    namespace board {
        constexpr float BOARD_WIDTH = 1000.0f;
        constexpr float BOARD_SPECIAL_WIDTH = 300.0f;
        constexpr float ROW_SPACING = 30.0f;
        constexpr float START_X = 750.0f;
        constexpr float START_SPECIAL_X = 300.0f;
        constexpr float START_Y = 600.0f;
        constexpr float CENTER_Y_SPACING = 50.0f;
    }


    inline void apply_resolution(Res res) {
        SetWindowSize(res.width, res.height);
        if (!IsWindowFullscreen()) {
            SetWindowPosition(
                (GetMonitorWidth(GetCurrentMonitor()) - res.width) / 2,
                (GetMonitorHeight(GetCurrentMonitor()) - res.height) / 2
            );
        }
    }

    inline Vector2 get_virtual_mouse() {
        Vector2 mouse = GetMousePosition();
        return {
            mouse.x * (static_cast<float>(VIRTUAL_WIDTH) / GetScreenWidth()),
            mouse.y * (static_cast<float>(VIRTUAL_HEIGHT) / GetScreenHeight())
        };
    }
}

#endif //PROJECT_NULL_RENDER_CONFIG_H