#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <iostream>

class Player {
protected:
    std::string name;
    double balance;

public:
    Player(const std::string& name_ = "Player", double balance_ = 1000) : name(name_), balance(balance_) {}
    virtual ~Player() = default;

    virtual std::istream& read(std::istream& is) = 0;
    virtual std::ostream& write(std::ostream& os) = 0;
    friend std::istream& operator>>(std::istream& is, Player& p) {
      return p.read(is);
    }
    friend std::ostream& operator<<(std::ostream& os, Player& p) {
      return p.write(os);
    }
};

#endif // PLAYER_H
