#ifndef PROJECT_NULL_WIDGET_MANAGER_CONSTRUCTION_H
#define PROJECT_NULL_WIDGET_MANAGER_CONSTRUCTION_H
#include "widget_manager.h"
#include "core/factory.h"
#include "visual/widgets/construction/widget_deck_list.h"

class widget_manager_construction : public widget_manager {
    public:
    widget_manager_construction(factory& data_factory);

    void update(float dt) override;
    void draw(renderer& ren) const override;

    private:
    std::unordered_map<const card*, std::unique_ptr<widget_full_card>> card_cache;

    std::unique_ptr<widget_card_pool> card_pool_view;
    std::unique_ptr<widget_deck_list> deck_list_view_;

};

#endif //PROJECT_NULL_WIDGET_MANAGER_CONSTRUCTION_H