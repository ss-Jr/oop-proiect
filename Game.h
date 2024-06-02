#ifndef GAME_H
#define GAME_H

#include <array>
#include <SFML/Graphics.hpp>
#include "Location.h"
#include "Button.h"
#include "Buttons.h"

class Game : public Location {
private:
    static std::vector<double> bet_amounts;
    sf::Texture game_texture;
    sf::Vector2u game_texture_size;
    sf::Sprite game_sprite;
    std::array<char, 3> reels;
    std::array<char, 4> symbols = { 'a', 'b', 'c', 'd' };

public:
    Game(const std::string& name_ = "Slot Game");

    Location* clone() const override;

    Game(const Game& other);

    Game& operator=(const Game& other);

    ~Game();

    void spin();

    bool checkWin();

    bool displayResult();

    void run() override {}

    void run(sf::RenderWindow& window, Buttons& buttons) override;

    friend std::ostream& operator<<(std::ostream& os, const Game& g);
};

#endif // GAME_H
