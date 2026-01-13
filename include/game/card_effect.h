#ifndef PROJECT_NULL_CARD_EFFECT_H
#define PROJECT_NULL_CARD_EFFECT_H
#include "card.h"
#include <string>

class card_effect : public card {
    private:
    std::string effect;

    public:
    card_effect(int id, std::string name, std::string effect);
    void display() const override;
    std::string get_effect() const;

};

#endif //PROJECT_NULL_CARD_EFFECT_H