#ifndef PROJECT_NULL_MENU_STATE_H
#define PROJECT_NULL_MENU_STATE_H
#include "state.h"
#include "visual/renderer.h"

class menu_state : public state {
public:
    void handle_input(state_manager& manager) override;
    void update(float dt) override;
    void render(renderer& renderer) override;
    std::string get_name() const override {return "Menu"; };

private:
    bool show_start_screen = true;
};



#endif //PROJECT_NULL_MENU_STATE_H