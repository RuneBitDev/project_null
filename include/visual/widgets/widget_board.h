#ifndef PROJECT_NULL_WIDGET_BOARD_H
#define PROJECT_NULL_WIDGET_BOARD_H
#include <vector>
#include "widget.h"
#include "widget_row.h"
#include "widget_special_row.h"

class widget_board : public widget {
    public:
    widget_board();

    void update_from_game(const board& b);

    void update() override;
    void draw() const override;

private:
    std::vector<widget_row> unit_rows;
    widget_special_row special_row;
};

#endif //PROJECT_NULL_WIDGET_BOARD_H