#include "game/engine/state_manager.h"

#include <iostream>

void state_manager::push_state(std::unique_ptr<state> state) {
    states.push(std::move(state));
    std::cout << "Successfully pushed state: " << states.top()->get_name() << std::endl;
}

void state_manager::pop_state() {
    if (!states.empty()) states.pop();
}

void state_manager::change_state(std::unique_ptr<state> state) {
    if (!states.empty()) states.pop();
    push_state(std::move(state));
}

void state_manager::update(float dt) {
    if (!states.empty()) states.top()->update(dt);
}

void state_manager::render(renderer& renderer) {
    if (!states.empty()) states.top()->render(renderer);
}

void state_manager::handle_input() {
    if (!states.empty()) states.top()->handle_input(*this);
}
