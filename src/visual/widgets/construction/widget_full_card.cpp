#include "visual/widgets/construction/widget_full_card.h"
#include "visual/texture_factory.h"

widget_full_card::widget_full_card(const card *c_ptr, card_detail card_detail)
    : card_data(c_ptr), c_detail(card_detail) {
    if (texture_factory::instance) {
        this->card_texture = texture_factory::instance->get_texture_for_card(c_ptr->get_id());
    }
}

void widget_full_card::update(float dt) {
}

void widget_full_card::draw() const {
    draw_at(this->card_bounds);
}

void widget_full_card::draw_at(Rectangle target_bounds) const {

    switch (c_detail) {
        case card_detail::MIN:
            DrawRectangleRec(target_bounds, Fade(GREEN, 0.1f));
            DrawRectangle(target_bounds.x, target_bounds.y, 5, target_bounds.height, GREEN);
            DrawText(card_data->get_name().c_str(), target_bounds.x + 15, target_bounds.y + 10, 20, GREEN);
            // maybe show quantity?
            break;
        case card_detail::MAX:
            DrawRectangleLinesEx(target_bounds, 2, GREEN);

            int textWidth = MeasureText(card_data->get_name().c_str(), 10);
            float posX = target_bounds.x + (target_bounds.width / 2.0f) - (textWidth / 2.0f);
            float posY = target_bounds.y + target_bounds.height - 25.0f;

            DrawText(card_data->get_name().c_str(), static_cast<int>(posX), static_cast<int>(posY), 10, GREEN);

            // texture
            if (card_texture.id > 0) {
                DrawTexturePro(card_texture,
                    {0, 0, static_cast<float>(card_texture.width), static_cast<float>(card_texture.height)},
                    {target_bounds.x + 5, target_bounds.y + 5, target_bounds.width - 10, target_bounds.height - 40},
                    {0,0}, 0.0f, WHITE);
            }
    }
}
