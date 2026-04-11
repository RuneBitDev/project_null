#ifndef PROJECT_NULL_WIDGET_ROW_H
#define PROJECT_NULL_WIDGET_ROW_H
#include <vector>
#include "../widget.h"
#include "widget_card.h"
#include "core/types.h"
#include "core/components/board.h"
#include "visual/widget_manager.h"

class widget_row : public widget {
public:
    widget_row(row_side side, row_type);
    void update_row(const board& game_board, widget_manager& manager);

    void update(float dt) override;
    void draw() const override;

private:
    std::vector<widget_card*> card_view_ptrs;
    row_type type;
    row_side side;

    bool has_score = true;
    int current_score = 0;
    std::string label;
    Rectangle row_bounds;
};

#endif //PROJECT_NULL_WIDGET_ROW_H