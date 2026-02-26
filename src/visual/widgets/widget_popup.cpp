#include "visual/widgets/widget_popup.h"
#include <utility>

#include "visual/render_config.h"

widget_popup::widget_popup(std::string text, Color color, float duration, popup_type popup_type)
    : message(std::move(text)), theme_color(color), p_type(popup_type), lifetime(duration), max_lifetime(duration) {}


void widget_popup::update(float dt) {
    lifetime -= dt;

    // fade
    if (lifetime > max_lifetime - 0.5f) {
        alpha = (max_lifetime - lifetime) * 2.0f;
    } else if (lifetime < 0.5f) {
        alpha = lifetime * 2.0f;
    } else {
        alpha = 1.0f;
    }
}

void widget_popup::draw() const {
    float screen_w = render_config::VIRTUAL_WIDTH;
    float screen_h = render_config::VIRTUAL_HEIGHT;
    Rectangle bar{};
    switch (p_type) {
        case popup_type::BANNER: bar = { 0, screen_h / 2 - 100, screen_w, 200 };
    }


    DrawRectangleRec(bar, Fade(BLACK, alpha * 0.8f));
    DrawRectangleLinesEx(bar, 4, Fade(theme_color, alpha));

    int fontSize = 80;
    int textWidth = MeasureText(message.c_str(), fontSize);
    DrawText(message.c_str(), (screen_w / 2) - (textWidth / 2), (screen_h / 2) - (fontSize / 2), fontSize, Fade(theme_color, alpha));

}
