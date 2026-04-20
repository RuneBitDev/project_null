#include "visual/managers/widget_manager_construction.h"

widget_manager_construction::widget_manager_construction(factory &data_factory) {
    // fill card cache with widget_card
    for (auto& card_ptr : data_factory.get_card_library()) {
        const card* raw_ptr = card_ptr.get();

        // initial context for construction library
        card_context ctx;
        ctx.face_up = true;
        ctx.detail = card_detail::MAX;
        ctx.position = card_position::POOL;

        card_cache[raw_ptr] = std::make_unique<widget_card>(raw_ptr, ctx);
    }

    Rectangle grid_bounds { 100, 150, 1200, 1000 };

    // iterate through factions and collect widget pointers
    for (int i = 0; i < 8; i++) {
        faction f = static_cast<faction>(i);
        std::vector<widget_card*> faction_widgets;

        for (auto& [card_ptr, widget] : card_cache) {
            if (card_ptr->get_faction() == f || card_ptr->get_faction() == faction::NEUTRAL) {
                faction_widgets.push_back(widget.get());
            }
        }

        card_pool_view.emplace(std::piecewise_construct,
            std::forward_as_tuple(f),
            std::forward_as_tuple(f, faction_widgets, grid_bounds));
    }

    // initialize deck_list
    //deck_list_view = std::make_unique<widget_deck_list>(/* bounds, etc */);

    clear_buttons();

    manage_button("FAC_PREV", "<", CLICKABLE, 0, { 450, 100, 50, 50 });
    manage_button("FAC_NEXT", ">", CLICKABLE, 0, { 800, 100, 50, 50 });
    manage_button("BACK", "BACK", CLICKABLE, 0, { 100, 100, 300, 100 });
}

void widget_manager_construction::update(float dt) {

    int faction_idx = static_cast<int>(current_faction);

    if (is_button_triggered("FAC_PREV")) {
        faction_idx = (faction_idx - 1 + 7) % 7;
        current_faction = static_cast<faction>(faction_idx);
    }
    if (is_button_triggered("FAC_NEXT")) {
        faction_idx = (faction_idx + 1) % 7;
        current_faction = static_cast<faction>(faction_idx);
    }

    auto it = card_pool_view.find(current_faction);
    if (it != card_pool_view.end()) {
        it->second.update(dt);

    }

    if (deck_list_view) deck_list_view->update(dt);
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
