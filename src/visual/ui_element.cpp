#include "visual/ui_element.h"
#include "visual/render_config.h"

bool ui_element::button_rec(Rectangle rec) {

    if (CheckCollisionPointRec(render_config::get_virtual_mouse(), rec)) {
        return IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }
    return false;
}
