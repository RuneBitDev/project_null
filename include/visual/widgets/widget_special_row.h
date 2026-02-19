#ifndef PROJECT_NULL_WIDGET_SPECIAL_ROW_H
#define PROJECT_NULL_WIDGET_SPECIAL_ROW_H
#include "widget.h"
#include "widget_card.h"
#include "core/components/board.h"

class widget_special_row : public widget {
    public:
    widget_special_row() = default;

    void update_from_game(const board& game_board);

    void update() override;
    void draw() const override;

private:
    std::vector<widget_card> active_specials;
    Rectangle bounds;
};


#endif //PROJECT_NULL_WIDGET_SPECIAL_ROW_H