#ifndef PROJECT_NULL_START_STATE_H
#define PROJECT_NULL_START_STATE_H
#include "state.h"
#include "visual/texture_factory.h"
#include "core/factory.h"

class start_state : public state {
public:
    start_state(factory& game_factory, texture_factory& texture_factory);
    ~start_state();

    void handle_input(state_manager& manager) override;
    void update(float dt, renderer& renderer) override;
    void render(renderer& renderer) override;
    std::string get_name() const override {return "Start"; };
private:
    factory& game_factory;
    texture_factory& tex_factory;

};

#endif