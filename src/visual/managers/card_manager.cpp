#include "visual/managers/card_manager.h"

void card_manager::update(float dt) {
    for (auto& [key, value] : card_widgets) {
        value.update(dt);
    }
}


// ---------------------- CARD WIDGETS ---------------------------
widget_card* card_manager::manage_card_widget(const card *card_ptr, card_context card_ctx) {
    auto it = card_widgets.find(card_ptr);
    if (it == card_widgets.end()) {
        auto result = card_widgets.emplace(card_ptr, widget_card(card_ptr, card_ctx));
        return &result.first->second;
    }

    it->second.sync_card_context(card_ctx);
    return &it->second;
}

void card_manager::draw_card_widgets() {
    for (const auto& [key, value] : card_widgets) {
        value.draw();
    }
}

void card_manager::clear_card_widgets() {
    card_widgets.clear();
}