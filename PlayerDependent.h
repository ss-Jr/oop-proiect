#ifndef PLAYERDEPENDENT_H
#define PLAYERDEPENDENT_H

#include "Player.h"
#include <string>
#include <iostream>

class PlayerDependent : public Player {
  public:
    PlayerDependent() = default;
    ~PlayerDependent() = default;
 
    std::istream& read(std::istream& is) override;
    std::ostream& write(std::ostream& os) override;
};

#endif // PLAYERDEPENDENT_H
