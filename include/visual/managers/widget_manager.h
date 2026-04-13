#ifndef PROJECT_NULL_WIDGET_MANAGER_H
#define PROJECT_NULL_WIDGET_MANAGER_H
#include "visual/renderer.h"

class widget_manager {
    public:
    virtual ~widget_manager() = default;

    virtual void update(float dt) = 0;
    virtual void draw(renderer& ren) const = 0;

    virtual void clear() = 0;
    virtual void handle_input() = 0;
};

#endif //PROJECT_NULL_WIDGET_MANAGER_H