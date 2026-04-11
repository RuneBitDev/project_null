#include "../../../../include/visual/widgets/match/widget_popup.h"

#include <cmath>
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
    float horizon_y = render_config::board::START_Y;

    Rectangle bar{};
    if (p_type == popup_type::BANNER) {
        bar = { 0, horizon_y - 100, screen_w, 200 };
    }

    // scanline background
    DrawRectangleRec(bar, Fade(BLACK, alpha * 0.85f));
    for (float i = bar.y; i < bar.y + bar.height; i += 8) {
        DrawRectangle(bar.x, i, bar.width, 2, Fade(theme_color, alpha * 0.1f));
    }

    // border
    float pulse = (sinf(GetTime() * 10.0f) * 0.5f + 0.5f); // Fast pulse for alerts
    DrawRectangleLinesEx(bar, 2, Fade(theme_color, alpha * 0.6f));
    DrawRectangle(bar.x, bar.y, bar.width, 4, Fade(theme_color, alpha * (0.4f + pulse * 0.4f)));
    DrawRectangle(bar.x, bar.y + bar.height - 4, bar.width, 4, Fade(theme_color, alpha * (0.4f + pulse * 0.4f)));

    // text
    int fontSize = 90;
    int textWidth = MeasureText(message.c_str(), fontSize);

    float jitter = (alpha > 0.9f) ? GetRandomValue(-2, 2) : 0;
    DrawText(message.c_str(),
             (screen_w / 2) - (textWidth / 2) + 4 + jitter,
             horizon_y - (fontSize / 2) + 4,
             fontSize, Fade(BLACK, alpha));

    DrawText(message.c_str(),
             (screen_w / 2) - (textWidth / 2) + jitter,
             horizon_y - (fontSize / 2),
             fontSize, Fade(theme_color, alpha));

    // sub-text
    const char* deco = ">> SYSTEM_NOTIFICATION_CRITICAL <<";
    int decoSize = 20;
    int decoWidth = MeasureText(deco, decoSize);
    DrawText(deco, (screen_w / 2) - (decoWidth / 2), bar.y + 10, decoSize, Fade(theme_color, alpha * 0.5f));
}
