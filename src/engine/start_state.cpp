#include "engine/start_state.h"
#include "engine/menu_state.h"
#include "engine/state_manager.h"

start_state::start_state(factory &game_factory, texture_factory &texture_factory)
    : game_factory(game_factory), tex_factory(texture_factory) {}

start_state::~start_state() = default;


void start_state::handle_input(state_manager &manager) {
    if (IsKeyPressed(KEY_ENTER)) {
        manager.change_state(std::make_unique<menu_state>(game_factory, tex_factory));
    }
}

void start_state::render(renderer &renderer) {
    renderer.draw_start_screen();
}

void start_state::update(float dt, renderer &renderer) {
}
