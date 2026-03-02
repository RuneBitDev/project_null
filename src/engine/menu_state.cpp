#include "engine/menu_state.h"
#include "engine/game_state.h"
#include "engine/state_manager.h"


menu_state::menu_state(player p1, player p2)
    : p1(std::move(p1)), p2(std::move(p2)) {
}



void menu_state::handle_input(state_manager &manager) {

    if (show_start_screen) {
        if (IsKeyPressed(KEY_ENTER)) show_start_screen = false;
    }

    if (start_button_is_pressed) {
        manager.change_state(std::make_unique<game_state>(std::move(p1), std::move(p2)));
    }

}

void menu_state::update(float dt, renderer& renderer) {
    if (!widgets_initialized) {
        renderer.init_menu_widgets();
        widgets_initialized = true;
    }
    if (!show_start_screen && renderer.is_button_triggered("START")) {
        start_button_is_pressed = true;
    }
    renderer.update_widgets(dt);
}

void menu_state::render(renderer& renderer) {
    if (show_start_screen) {
        renderer.draw_start_screen();
    } else {
        renderer.draw_menu();

    }
}
