#ifndef PROJECT_NULL_STATE_MANAGER_H
#define PROJECT_NULL_STATE_MANAGER_H
#include <memory>
#include <stack>
#include "state.h"

class state_manager {
    private:
    std::stack<std::unique_ptr<state>> states;

    public:
    void push_state(std::unique_ptr<state> state);
    void pop_state();
    void change_state(std::unique_ptr<state> state);
    void update(float dt);
    void render();
    void handle_input();


};

#endif //PROJECT_NULL_STATE_MANAGER_H