#ifndef PROJECT_NULL_DECK_STATE_H
#define PROJECT_NULL_DECK_STATE_H
#include "core/factory.h"
#include "engine/state.h"

class deck_state : public state {
public:
    deck_state(factory &factory, texture_factory &texture_factory);
    ~deck_state();

    void handle_input(state_manager& manager) override;
    void update(float dt, renderer& renderer) override;
    void render(renderer& ren) override;

    std::string get_name() const override {return "Deck"; };

private:
    factory& data_factory;
    texture_factory& tex_factory;

    std::vector<card*> card_pool;

    bool widgets_initialized = false;
    bool is_back_button_pressed = false;

};

#endif //PROJECT_NULL_DECK_STATE_H