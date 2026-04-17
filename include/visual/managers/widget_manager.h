#ifndef PROJECT_NULL_WIDGET_MANAGER_H
#define PROJECT_NULL_WIDGET_MANAGER_H
#include "visual/widgets/widget_button.h"

class renderer;

class widget_manager {
public:
    virtual ~widget_manager() = default;

    virtual void update(float dt) = 0;
    virtual void draw(renderer& ren) const = 0;

    virtual void clear() = 0;
    virtual void handle_input() = 0;

    virtual void add_popup(const std::string& text, Color color, float duration, popup_type p_type) {}

    widget_button* manage_button(const std::string& id, const char* text, button_type type, int key, Rectangle bounds) {
        auto& btn = button_widgets[id];
        btn.init_button(text, type, key, bounds);
        return &btn;
    }

    widget_button* get_button(const std::string& id) {
        auto it = button_widgets.find(id);
        return (it != button_widgets.end()) ? &it->second : nullptr;
    }

    bool is_button_triggered(const std::string& id) {
        auto* btn = get_button(id);
        return btn->is_triggered();
    }

    void set_button_enabled(const std::string& id, bool enabled) {
        auto* btn = get_button(id);
        if (btn) btn->set_enabled(enabled);
    }

    void clear_buttons() { button_widgets.clear(); }

protected:
    std::map<std::string, widget_button> button_widgets;

    void update_buttons(float dt) {
        for (auto& [id, btn] : button_widgets) btn.update(dt);
    }

    void draw_buttons() const {
        for (const auto& [id, btn] : button_widgets) btn.draw();
    }

};

#endif //PROJECT_NULL_WIDGET_MANAGER_H