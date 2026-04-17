#ifndef PROJECT_NULL_MENU_STATE_H
#define PROJECT_NULL_MENU_STATE_H
#include "state.h"
#include "core/factory.h"
#include "visual/texture_factory.h"
#include "visual/managers/widget_manager_menu.h"


class menu_state : public state {
public:
    menu_state(factory& game_factory, texture_factory& texture_factory);
    ~menu_state();

    void handle_input(state_manager& manager) override;
    void update(float dt, renderer& renderer) override;
    void render(renderer& renderer) override;
    std::string get_name() const override {return "Menu"; };

private:
    factory& data_factory;
    texture_factory& tex_factory;

    std::vector< std::string > available_factions = {"arasaka", "afterlife", "aldecaldos", "maelstrom", "vodoo_boys", "fia", "barghest"};

    std::string p1_faction = "arasaka";
    std::string p2_faction = "arasaka";

    int p1_select = 0;
    int p2_select = 0;
};



#endif //PROJECT_NULL_MENU_STATE_H