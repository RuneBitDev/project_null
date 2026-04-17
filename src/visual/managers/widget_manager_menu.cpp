#include "visual/managers/widget_manager_menu.h"
#include "visual/render_config.h"

widget_manager_menu::widget_manager_menu() {

    clear_buttons();
    float centerX = render_config::VIRTUAL_WIDTH / 2.0f;

    float imgWidth = 1200.0f;
    float imgHeight = 400.0f;
    float arrowW = 80.0f;
    float arrowH = 120.0f;

    // p2 top section
    float p2_y = 180.0f;
    Rectangle p2_rect = { centerX - (imgWidth / 2.0f), p2_y, imgWidth, imgHeight };
    manage_button("P2_PREV", "<", CLICKABLE, 0, { p2_rect.x - arrowW - 40, p2_y + (imgHeight/2 - arrowH/2), arrowW, arrowH });
    manage_button("P2_NEXT", ">", CLICKABLE, 0, { p2_rect.x + imgWidth + 40, p2_y + (imgHeight/2 - arrowH/2), arrowW, arrowH });

    // p1 bottom section
    float p1_y = 860.0f;
    Rectangle p1_rect = { centerX - (imgWidth / 2.0f), p1_y, imgWidth, imgHeight };
    manage_button("P1_PREV", "<", CLICKABLE, 0, { p1_rect.x - arrowW - 40, p1_y + (imgHeight/2 - arrowH/2), arrowW, arrowH });
    manage_button("P1_NEXT", ">", CLICKABLE, 0, { p1_rect.x + imgWidth + 40, p1_y + (imgHeight/2 - arrowH/2), arrowW, arrowH });

    Rectangle start_rect = { centerX - 250, 670, 500, 100 };
    manage_button("START", "INITIALIZE OPERATION", CLICKABLE, 0, start_rect);
    Rectangle deck_rect = {2000, 800, 400, 100};
    manage_button("DECK", "EDIT DECK", CLICKABLE, 0, deck_rect);
}

void widget_manager_menu::update(float dt) {
    update_buttons(dt);
}

void widget_manager_menu::draw(renderer &ren) const {
    draw_buttons();
}

void widget_manager_menu::handle_input() {
}

void widget_manager_menu::clear() {
    clear_buttons();
}
