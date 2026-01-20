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


    inline void apply_resolution(Res res) {
        SetWindowSize(res.width, res.height);

        SetWindowPosition(GetMonitorWidth(0)/2 - res.width/2, GetMonitorHeight(0)/2 - res.height/2);
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