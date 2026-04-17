#ifndef PROJECT_NULL_WIDGET_FULL_CARD_H
#define PROJECT_NULL_WIDGET_FULL_CARD_H
#include "core/components/card.h"
#include "visual/ui_types.h"
#include "visual/widgets/widget.h"

enum class card_detail {MIN, MAX};

class widget_full_card : public widget {
public:
    widget_full_card(const card* c_ptr);
    void update(float dt) override;
    void draw() const override;

    void draw_at(Rectangle target_bounds) const;

    void set_detail(card_detail new_c_detail) { c_detail = new_c_detail; }

    void set_bounds(Rectangle new_bounds) { card_bounds = new_bounds; }
    Rectangle get_bounds() const { return card_bounds; }

private:
    const card* card_data;
    card_detail c_detail;

    Texture2D card_texture;
    Rectangle card_bounds;

};

#endif //PROJECT_NULL_WIDGET_FULL_CARD_H