#ifndef PROJECT_NULL_WIDGET_GAME_LOG_H
#define PROJECT_NULL_WIDGET_GAME_LOG_H
#include "raylib.h"
#include "../widget.h"
#include "visual/render_config.h"

class widget_game_log : public widget {
public:
    void update(float dt) override;
    void draw() const override;

private:
    Rectangle bounds = render_config::game_log::CONSOLE;
    bool is_collapsed = true;
    int scroll_offset = 0;
    float font_size = 16.0f;

    Rectangle get_toggle_bounds() const;

};

#endif //PROJECT_NULL_WIDGET_GAME_LOG_H