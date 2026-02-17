#ifndef PROJECT_NULL_WIDGET_CARD_H
#define PROJECT_NULL_WIDGET_CARD_H
#include "raylib.h"
#include "widget.h"
#include "core/components/card.h"
#include "visual/ui_types.h"

class widget_card : public widget {
    public:
    card_widget(const std::unique_ptr<card>& model, Rectangle bounds, bool face_up);
    void update();
    void draw(const ui_card& card) const;

private:
    const std::unique_ptr<card>& card_ptr;
    Rectangle base_bounds;
    mutable Rectangle current_bounds;

    bool face_up;
    bool hovered = false;
};
#endif //PROJECT_NULL_WIDGET_CARD_H