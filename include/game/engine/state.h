#ifndef PROJECT_NULL_STATE_H
#define PROJECT_NULL_STATE_H
#include <string>
#include "visual/renderer.h"

class state_manager;
class renderer;

class state {
public:
    virtual ~state() = default;

    virtual void handle_input(state_manager& manager) = 0;
    virtual void update(float dt) = 0;
    virtual void render(renderer& renderer) = 0;

    virtual std::string get_name() const = 0;
};

#endif //PROJECT_NULL_STATE_H