#include "Project.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

Project::Project(std::unique_ptr<Application> ptr_) : ptr(std::move(ptr_)) {}

void Project::start() const {
    Buttons b;
    sf::RenderWindow w;
    ptr->run(w, b);
}

Project& Project::operator=(Project other) {
    swap(*this, other);
    return *this;
}

void swap(Project& p1, Project& p2) {
    std::swap(p1.ptr, p2.ptr);
}

void Project::setPlayer(Player* p) {
  players.push_back(p);
}

std::vector<Player*> Project::getPlayers() {
  return players;
}
