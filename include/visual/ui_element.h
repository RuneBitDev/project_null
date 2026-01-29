#ifndef PROJECT_NULL_UI_ELEMENT_H
#define PROJECT_NULL_UI_ELEMENT_H
#include "raylib.h"
#include "ui_types.h"


class ui_element {
public:
    bool button_rec(Rectangle rec);
    bool card_rec(Rectangle rec);
    bool button_pass(Rectangle rec);

    void update_button(button& btn);
    void update_card(ui_card& card);
private:
    float hold_timer = 0.0f;
};

#endif //PROJECT_NULL_UI_ELEMENT_H