#ifndef PROJECT_NULL_CARD_MANAGER_H
#define PROJECT_NULL_CARD_MANAGER_H
#include "visual/widgets/match/widget_card.h"

#include <map>

class card_manager {
    public:
    void update(float dt);

    widget_card* manage_card_widget(const card* card_ptr, card_context card_ctx);
    void draw_card_widgets();
    void clear_card_widgets();

private:
    std::map<const card*, widget_card> card_widgets;
};

#endif //PROJECT_NULL_CARD_MANAGER_H