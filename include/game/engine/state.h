#ifndef PROJECT_NULL_STATE_H
#define PROJECT_NULL_STATE_H
#include <string>

class state_manager;

class state {
public:
    virtual ~state() = default;

    virtual void handle_input(state_manager& manager) = 0;
    virtual void update(float dt) = 0;      // delta time
    virtual void render() = 0;

    virtual std::string get_name() const = 0;
};

#endif //PROJECT_NULL_STATE_H