#ifndef PROJECT_NULL_WIDGET_DECK_GRID_H
#define PROJECT_NULL_WIDGET_DECK_GRID_H
#include <vector>

#include "raylib.h"
#include "../widget.h"
#include "../match/widget_card.h"

class widget_deck_grid : public widget {
public:
    void init_deck_grid();

    void update(float dt) override;
    void draw() const override;

private:
    std::vector<widget_card*> deck_view_ptrs;
    Rectangle grid_bounds {};

};

#endif //PROJECT_NULL_WIDGET_DECK_GRID_H