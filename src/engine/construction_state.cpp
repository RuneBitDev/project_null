#include "engine/construction_state.h"

#include "engine/menu_state.h"
#include "engine/state_manager.h"

construction_state::construction_state(factory &factory, texture_factory &texture_factory)
    : data_factory(factory), tex_factory(texture_factory) {
    for (auto& c : data_factory.get_card_library()) {
        card_pool.push_back(c.get());
    }
}

construction_state::~construction_state() {
    tex_factory.unload_transient();
}

void construction_state::handle_input(state_manager &manager) {
    if (is_back_button_pressed) {
        manager.change_state(std::make_unique<menu_state>(data_factory, tex_factory));
    }
}

void construction_state::update(float dt, renderer &renderer) {

    if (!widgets_initialized) {
        renderer.init_deck_builder_widgets("TEST", card_pool);
        widgets_initialized = true;
    }

    renderer.update_deck_builder_widgets(dt);

    if (renderer.is_button_triggered("BACK")) {
        is_back_button_pressed = true;
    }
}

void construction_state::render(renderer &ren) {

    ren.draw_deck_builder();
}

