#ifndef PROJECT_NULL_WIDGET_CARD_H
#define PROJECT_NULL_WIDGET_CARD_H
#include "raylib.h"
#include "widget.h"
#include "core/components/card.h"
#include "visual/ui_types.h"

class widget_card : public widget {
    public:
    widget_card(const card* c_ptr, card_context card_ctx);
    void update(float dt) override;
    void draw() const override;

    void sync_card_context(const card_context& card_ctx);
    void set_bounds(Rectangle new_bounds);

private:
    const card* card_data;
    Rectangle base_bounds;
    Rectangle current_bounds;
    Rectangle target_bounds;

    card_context card_ctx;
    bool hovered = false;
    float hover_timer = 0.0f;
    bool triggered = false;
};
#endif //PROJECT_NULL_WIDGET_CARD_H