#ifndef PROJECT_NULL_MENU_STATE_H
#define PROJECT_NULL_MENU_STATE_H
#include "state.h"
#include "core/factory.h"


class menu_state : public state {
public:
    menu_state(factory& game_factory);
    void handle_input(state_manager& manager) override;
    void update(float dt, renderer& renderer) override;
    void render(renderer& renderer) override;
    std::string get_name() const override {return "Menu"; };

private:
    factory& data_factory;

    std::vector< std::string > available_factions = {"arasaka", "afterlife", "aldecaldos", "maelstrom", "vodoo boys", "nusa", "barghest"};
    bool show_start_screen = true;
    bool widgets_initialized = false;
    bool start_button_is_pressed = false;
    std::string p1_faction;
    std::string p2_faction;
};



#endif //PROJECT_NULL_MENU_STATE_H