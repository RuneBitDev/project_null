#ifndef PROJECT_NULL_WIDGET_DECK_VIEW_H
#define PROJECT_NULL_WIDGET_DECK_VIEW_H
#include "widget_full_card.h"
#include "visual/widgets/widget.h"

class widget_deck_list : public widget {
public:
    widget_deck_list(const std::vector<card*>& deck_cards);
    void update(float dt) override;
    void draw() const override;

private:
    std::vector<widget_full_card> deck_entries;
    Rectangle bounds;

};

#endif //PROJECT_NULL_WIDGET_DECK_VIEW_H