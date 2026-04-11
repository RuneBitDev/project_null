#ifndef PROJECT_NULL_WIDGET_MANAGER_H
#define PROJECT_NULL_WIDGET_MANAGER_H
#include "widgets/match/widget_card.h"
#include "widgets/widget_button.h"
#include <map>

class widget_manager {
    public:
    void update(float dt);

    widget_card* manage_card_widget(const card* card_ptr, card_context card_ctx);
    void draw_card_widgets();
    void clear_card_widgets();

    widget_button* manage_button_widget(const std::string& id, const char* text, button_type, int key, Rectangle bounds);
    widget_button* get_button(const std::string& id);
    void draw_buttons();
    void clear_button_widgets();

private:
    std::map<const card*, widget_card> card_widgets;
    std::map<std::string, widget_button> button_widgets;
};

#endif //PROJECT_NULL_WIDGET_MANAGER_H