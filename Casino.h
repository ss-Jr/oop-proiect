#ifndef CASINO_H
#define CASINO_H

#include <SFML/Graphics.hpp>
#include "Location.h"
#include "Machine.h"
#include "Player.h"

class Casino : public Location {
private:
    std::vector<Machine> machines;
    std::vector<Player> players;
    sf::Texture casino_texture;
    sf::Vector2u casino_texture_size;
    sf::Sprite casino_sprite;

public:
    Casino(const std::string& name_ = "Casino");

    Location* clone() const override;

    Casino(const Casino& other);

    Casino& operator=(const Casino& other);

    ~Casino();

    void run() override {}

    void run(sf::RenderWindow& window, Buttons& buttons) override;

    friend std::ostream& operator<<(std::ostream& os, const Casino& c);
};

#endif // CASINO_H
