#ifndef PROJECT_NULL_WIDGET_BUTTON_H
#define PROJECT_NULL_WIDGET_BUTTON_H
#include "raylib.h"
#include "widget.h"
#include "visual/ui_types.h"

class widget_button : public widget {
public:
    void init_button(const char* text, button_type b_type, int key, Rectangle bounds);

    void update(float dt) override;
    void draw() const override;

    bool is_triggered() const { return triggered; }
    void set_enabled(bool val) { enabled = val; }

private:
    Rectangle bounds{};
    button_type b_type;
    int shortcut_key{};
    const char* text{};

    bool enabled = true;
    bool is_hovered = false;
    bool is_down = false;
    float hold_progress = 0.0f;
    bool triggered = false;


};

#endif //PROJECT_NULL_WIDGET_BUTTON_H