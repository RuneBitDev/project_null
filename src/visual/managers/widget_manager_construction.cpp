#include "visual/managers/widget_manager_construction.h"

widget_manager_construction::widget_manager_construction(factory &data_factory) {
    // fill card cache
    for (auto& card_ptr : data_factory.get_card_library()) {
        const card* raw_ptr = card_ptr.get();
        card_cache[raw_ptr] = std::make_unique<widget_full_card>(raw_ptr);
    }

    Rectangle grid_bounds { 100, 150, 1200, 1000 };

    //iterate through all possible factions
    for (int i = 0; i < 8; i++) {
        faction f = static_cast<faction>(i);
        std::vector<widget_full_card*> faction_widgets;

        for (auto& [card_ptr, widget] : card_cache) {
            if (card_ptr->get_faction() == f || card_ptr->get_faction() == faction::NEUTRAL) {
                faction_widgets.push_back(widget.get());
            }
        }

        card_pool_view.emplace(std::piecewise_construct,
            std::forward_as_tuple(f),
            std::forward_as_tuple(f, faction_widgets, grid_bounds));
    }

    clear_buttons();
    Rectangle back_rect = { 100, 100, 300, 100 };
    manage_button("BACK", "BACK", CLICKABLE, 0, back_rect);
}

void widget_manager_construction::update(float dt) {
    auto it = card_pool_view.find(current_faction);
    if (it != card_pool_view.end()) {
        it->second.update(dt);
    }
    //deck_list_view->update(dt);
    update_buttons(dt);
}

void widget_manager_construction::draw(renderer &ren) const {
    draw_buttons();

    auto it = card_pool_view.find(current_faction);
    if (it != card_pool_view.end()) {
        it->second.draw();
    }

    // deck_list_view->draw();
}

void widget_manager_construction::clear() {
    clear_buttons();
}

void widget_manager_construction::handle_input() {
}
