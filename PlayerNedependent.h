#ifndef PLAYERNEDEPENDENT_H
#define PLAYERNEDEPENDENT_H

#include "Player.h"
#include <string>
#include <iostream>

class PlayerNedependent : public Player {
  public:
    PlayerNedependent() = default;
    ~PlayerNedependent() = default;
    
    std::istream& read(std::istream& is) override;
    std::ostream& write(std::ostream& os) override;
};
  
#endif // PLAYERNEDEPENDENT_H
