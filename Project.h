#ifndef PROJECT_H
#define PROJECT_H

#include <memory>
#include "Application.h"
#include "Player.h"
#include "PlayerDependent.h"
#include "PlayerNedependent.h"

class Project {
private:
    std::vector<Player*> players;
    std::unique_ptr<Application> ptr;
public:
    explicit Project(std::unique_ptr<Application> ptr_);

    void start() const;

    Project& operator=(Project other);

    friend void swap(Project& p1, Project& p2);

    void setPlayer(Player* p);
    std::vector<Player*> getPlayers();

    friend std::ostream& operator<<(std::ostream& os, Project& p) {
      os << "\nPlayeri dependenti:";
      for(auto e: p.getPlayers()) {
        if(auto const player = dynamic_cast<PlayerDependent*>(e)) {
          os << *player;
        }
      }
      os << "\nPlayeri nedependenti:";
      for(auto e: p.getPlayers()) {
        if(auto const player = dynamic_cast<PlayerNedependent*>(e)) {
          os << *player;
        }
      }
      return os;
    }
};

#endif // PROJECT_H
