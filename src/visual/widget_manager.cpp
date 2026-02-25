#include "visual/widget_manager.h"

widget_card* widget_manager::manage_card_widget(const card *card_ptr, card_context card_ctx) {
    auto it = card_widgets.find(card_ptr);
    if (it == card_widgets.end()) {
        auto result = card_widgets.emplace(card_ptr, widget_card(card_ptr, card_ctx));
        return &result.first->second;
    }

    it->second.sync_card_context(card_ctx);
    return &it->second;
}

void widget_manager::clear_card_widgets() {
    card_widgets.clear();
}
