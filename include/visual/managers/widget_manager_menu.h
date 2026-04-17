#ifndef PROJECT_NULL_WIDGET_MANAGER_MENU_H
#define PROJECT_NULL_WIDGET_MANAGER_MENU_H
#include "widget_manager.h"

class widget_manager_menu : public widget_manager {
    public:
    widget_manager_menu();

    void update(float dt) override;
    void draw(renderer &ren) const override;

    void handle_input() override;
    void clear() override;

private:

};

#endif //PROJECT_NULL_WIDGET_MANAGER_MENU_H