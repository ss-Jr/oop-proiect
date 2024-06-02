#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <iostream>

class Player {
private:
    std::string name;
    double balance;

public:
    Player(const std::string& name_ = "Player", double balance_ = 1000) : name(name_), balance(balance_) {}

    friend std::ostream& operator<<(std::ostream& os, const Player& p) {
        os << "Player " << p.name << " has a balance of $" << p.balance << ".\n";
        return os;
    }
};

#endif // PLAYER_H
