#ifndef PROJECT_NULL_CARD_H
#define PROJECT_NULL_CARD_H
#include <string>
#include <iostream>

class card {
    private:
    int id;
    std::string name;
    std::string faction;

    public:
    card(int id, std::string name, std::string faction);
    virtual ~card() = default;
    virtual void display() const;

    int get_id() const;
    std::string get_name() const;

};

#endif //PROJECT_NULL_CARD_H