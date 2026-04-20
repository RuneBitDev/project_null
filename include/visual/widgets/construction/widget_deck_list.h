#ifndef PROJECT_NULL_WIDGET_DECK_VIEW_H
#define PROJECT_NULL_WIDGET_DECK_VIEW_H

#include "visual/widgets/widget.h"
#include "visual/widgets/widget_card.h"

class widget_deck_list : public widget {
public:
    widget_deck_list(const std::vector<card*>& deck_cards);
    void update(float dt) override;
    void draw() const override;

private:
    std::vector<widget_card> deck_entries;
    Rectangle bounds;

};

#endif //PROJECT_NULL_WIDGET_DECK_VIEW_H