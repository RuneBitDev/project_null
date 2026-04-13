#ifndef PROJECT_NULL_WIDGET_DECK_H
#define PROJECT_NULL_WIDGET_DECK_H
#include <vector>
#include "../widget.h"
#include "widget_card.h"
#include "core/types.h"
#include "core/components/deck.h"
#include "../../managers/widget_manager_game.h"

class widget_deck : public widget {
public:
    void init_deck(row_side side);
    void update_from_deck(const deck& d, widget_manager_game& manager);

    void update(float dt) override;
    void draw() const override;

private:
    std::vector<widget_card*> deck_view_ptrs;

    Rectangle deck_bounds;
    int card_count = 0;
    row_side side;
};

#endif //PROJECT_NULL_WIDGET_DECK_H