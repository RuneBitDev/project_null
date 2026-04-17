#ifndef PROJECT_NULL_WIDGET_SPECIAL_ROW_H
#define PROJECT_NULL_WIDGET_SPECIAL_ROW_H
#include "../widget.h"
#include "widget_card.h"
#include "core/components/board.h"
#include "../../managers/card_manager.h"

class widget_special_row : public widget {
    public:
    widget_special_row() = default;

    void update_from_game(const board& game_board, card_manager& manager);

    void update(float dt) override;
    void draw() const override;

private:
    std::vector<widget_card*> special_row_ptrs;
    Rectangle bounds;
};


#endif //PROJECT_NULL_WIDGET_SPECIAL_ROW_H