#include "visual/widget_manager.h"

void widget_manager::update(float dt) {
    for (auto& [key, value] : card_widgets) {
        value.update(dt);
    }

    for (auto& [key, value] : button_widgets) {
        value.update(dt);
    }
}


// ---------------------- CARD WIDGETS ---------------------------
widget_card* widget_manager::manage_card_widget(const card *card_ptr, card_context card_ctx) {
    auto it = card_widgets.find(card_ptr);
    if (it == card_widgets.end()) {
        auto result = card_widgets.emplace(card_ptr, widget_card(card_ptr, card_ctx));
        return &result.first->second;
    }

    it->second.sync_card_context(card_ctx);
    return &it->second;
}

void widget_manager::draw_card_widgets() {
    for (const auto& [key, value] : card_widgets) {
        value.draw();
    }
}

void widget_manager::clear_card_widgets() {
    card_widgets.clear();
}


// ---------------------- BUTTON WIDGETS ---------------------------
widget_button* widget_manager::manage_button_widget(const std::string& id, const char* text, button_type type, int key, Rectangle bounds) {
    auto& btn = button_widgets[id];
    btn.init_button(text, type, key, bounds);
    return &btn;
}

widget_button* widget_manager::get_button(const std::string& id) {
    auto it = button_widgets.find(id);
    return (it != button_widgets.end()) ? &it->second : nullptr;
}

void widget_manager::draw_buttons() {
    for (auto& [id, btn] : button_widgets) {
        btn.draw();
    }
}

void widget_manager::clear_button_widgets() {
    button_widgets.clear();
}