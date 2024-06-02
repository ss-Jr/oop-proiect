#ifndef MACHINE_H
#define MACHINE_H

#include <SFML/Graphics.hpp>
#include "Location.h"
#include "Game.h"

class Machine : public Location {
private:
    std::vector<Game> games;
    sf::Texture machine_texture;
    sf::Vector2u machine_texture_size;
    sf::Sprite machine_sprite;

public:
    Machine(const std::string& name_ = "Slot Machine");

    Location* clone() const override;

    Machine(const Machine& other);

    Machine& operator=(const Machine& other);

    ~Machine();

    void run() override {}

    void run(sf::RenderWindow& window, Buttons& buttons) override;

    friend std::ostream& operator<<(std::ostream& os, const Machine& m);
};

#endif // MACHINE_H
