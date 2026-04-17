#ifndef PROJECT_NULL_STATE_H
#define PROJECT_NULL_STATE_H
#include <string>
#include "visual/managers/widget_manager.h"
#include "visual/renderer.h"

class state_manager;


class state {
public:
    virtual ~state() = default;

    virtual void handle_input(state_manager& manager) = 0;
    virtual void update(float dt, renderer& renderer) = 0;
    virtual void render(renderer& renderer) = 0;

    virtual std::string get_name() const = 0;

protected:
    std::unique_ptr<widget_manager> ui_manager;

    bool ui_button_clicked(const std::string& id) {
        return ui_manager && ui_manager->is_button_triggered(id);
    }



};

#endif //PROJECT_NULL_STATE_H