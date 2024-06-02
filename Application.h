#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics.hpp>
#include "Location.h"
#include "Casino.h"

class Application : public Location {
private:
    std::vector<Casino> casinos;
    sf::RenderWindow window;
    sf::Texture app_texture;
    sf::Vector2u app_texture_size;
    sf::Sprite app_sprite;

public:
    Application(const std::string& name_ = "Pacanea");

    Location* clone() const override;

    ~Application();

    Application& operator=(const Application& other);

    Application(const Application& other);

    //void run(sf::RenderWindow& window, Buttons& buttons) override {}

    void run() override;

    friend std::ostream& operator<<(std::ostream& os, const Application& a);
};

#endif // APPLICATION_H
