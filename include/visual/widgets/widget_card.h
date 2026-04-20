#ifndef PROJECT_NULL_WIDGET_CARD_H
#define PROJECT_NULL_WIDGET_CARD_H
#include "raylib.h"
#include "widget.h"
#include "core/components/card.h"
#include "visual/ui_types.h"

class widget_card : public widget {
public:
    widget_card(const card* c_ptr, const card_context &card_ctx);
    void update(float dt) override;
    void draw() const override;

    void sync_card_context(const card_context& card_ctx);
    void set_card_texture(Texture2D new_texture) { card_texture = new_texture; }

    Rectangle get_base_bounds() const { return base_bounds; }
    Rectangle get_current_bounds() const { return current_bounds; }
    void set_bounds(Rectangle new_bounds);

    void draw_at(Rectangle target_rect) const;


private:
    Texture2D card_texture;
    const card* card_data;

    Rectangle base_bounds;
    Rectangle current_bounds;
    Rectangle target_bounds;

    card_context card_ctx;
    bool hovered = false;
    float hover_timer = 0.0f;
    bool triggered = false;


    void draw_full_visual(Rectangle draw_rect) const;
    void draw_stat_banner(Vector2 pos, float width, float height, int value, Color base_color, Color value_color) const;
};
#endif //PROJECT_NULL_WIDGET_CARD_H