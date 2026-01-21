#ifndef PROJECT_NULL_RENDER_CONFIG_H
#define PROJECT_NULL_RENDER_CONFIG_H
#include "raylib.h"

namespace render_config {

    const int VIRTUAL_WIDTH = 2560;
    const int VIRTUAL_HEIGHT = 1440;

    struct Res {
        int width;
        int height;
        const char *name;
    };

    const Res screen_720p = {1280, 720, "1280x720"};
    const Res screen_1080p = {1920, 1080, "1920x1080"};
    const Res screen_1440p = {2560, 1440, "2560x1440"};

    namespace ui {
        const Rectangle START_BUTTON = { 1080, 600, 400, 100};
    }

    namespace card {
        constexpr float CARD_WIDTH = 100.0f;
        constexpr float CARD_HEIGHT = 150.0f;
    }

    namespace hand {
        constexpr float X_OFFSET = 700.0f;
        constexpr float Y_OFFSET = 1200.0f;
    }

    namespace graveyard {
        constexpr float GY_X = 2000.0f;
        constexpr float GY_Y = 600.0f;
    }

    namespace board {
        constexpr float BOARD_WIDTH = 1000.0f;
        constexpr float START_X = 750.0f;
        constexpr float START_Y_OPPONENT = 100.0f;
        constexpr float START_Y_PLAYER = 630.0f;
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
            mouse.x * ((float)VIRTUAL_WIDTH / GetScreenWidth()),
            mouse.y * ((float)VIRTUAL_HEIGHT / GetScreenHeight())
        };
    }
}

#endif //PROJECT_NULL_RENDER_CONFIG_H