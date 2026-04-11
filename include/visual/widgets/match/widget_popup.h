#ifndef PROJECT_NULL_WIDGET_POPUP_H
#define PROJECT_NULL_WIDGET_POPUP_H
#include <string>
#include "raylib.h"
#include "../widget.h"
#include "visual/ui_types.h"

class widget_popup : public widget {
public:
    widget_popup(std::string text, Color color, float duration, popup_type popup_type);

    void update(float dt) override;
    void draw() const override;

    bool is_finished() const { return lifetime <= 0.0f; }

private:
    std::string message;
    Color theme_color;
    popup_type p_type;

    float lifetime;
    float max_lifetime;
    float alpha = 0.0f;
};

#endif //PROJECT_NULL_WIDGET_POPUP_H