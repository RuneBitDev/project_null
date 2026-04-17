#include "engine/construction_state.h"

#include "engine/menu_state.h"
#include "engine/state_manager.h"

construction_state::construction_state(factory &factory, texture_factory &texture_factory)
    : data_factory(factory), tex_factory(texture_factory) {

    ui_manager = std::make_unique<widget_manager_construction>(factory);

    for (auto& c : data_factory.get_card_library()) {
        card_pool.push_back(c.get());
    }

}

construction_state::~construction_state() {
    tex_factory.unload_transient();
}

void construction_state::handle_input(state_manager &manager) {
    ui_manager->handle_input();
    if (ui_button_clicked("BACK")) {
        manager.change_state(std::make_unique<menu_state>(data_factory, tex_factory));
    }
}

void construction_state::update(float dt, renderer &renderer) {

    ui_manager->update(dt);

}

void construction_state::render(renderer &ren) {
    ui_manager->draw(ren);
    ren.draw_construction();
}

