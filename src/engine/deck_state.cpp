#include "engine/deck_state.h"

#include "engine/menu_state.h"
#include "engine/state_manager.h"

deck_state::deck_state(factory &factory, texture_factory &texture_factory)
    : data_factory(factory), tex_factory(texture_factory) {
    cards = {data_factory.get_special_library(), data_factory.get_unit_library()};
}

deck_state::~deck_state() {
    tex_factory.unload_transient();
}

void deck_state::handle_input(state_manager &manager) {
    if (is_back_button_pressed) {
        manager.change_state(std::make_unique<menu_state>(data_factory, tex_factory));
    }
}

void deck_state::update(float dt, renderer &renderer) {

    if (!widgets_initialized) {
        renderer.init_deck_builder_widgets();
        widgets_initialized = true;
    }

    renderer.update_deck_builder_widgets(dt);

    if (renderer.is_button_triggered("BACK")) {
        is_back_button_pressed = true;
    }
}

void deck_state::render(renderer &ren) {
    ren.draw_deck_builder(cards, 0);
}

