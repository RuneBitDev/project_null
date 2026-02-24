#ifndef PROJECT_NULL_WIDGET_H
#define PROJECT_NULL_WIDGET_H

class widget {
public:
    virtual ~widget() = default;
    virtual void update(float dt) = 0;
    virtual void draw() const = 0;
};

#endif //PROJECT_NULL_WIDGET_H