#include "Machine.h"
#include <iostream>

Machine::Machine(const std::string& name_) : Location(name_), games(std::vector<Game>(1, Game("Slot Game"))) {
    if (!machine_texture.loadFromFile("games.jpg")) {
        std::cout << "Could not load slot machine textures.\n";
    } else {
        machine_texture_size = machine_texture.getSize();
    }
}

Location* Machine::clone() const {
    return new Machine(*this);
}

Machine::Machine(const Machine& other) : Location(other), games(other.games), machine_texture(other.machine_texture), machine_texture_size(other.machine_texture_size), machine_sprite(other.machine_sprite) {
    std::cout << "Copy constructor Machine\n";
}

Machine& Machine::operator=(const Machine& other) {
    if (this == &other)
        return *this;
    Location::operator=(other);
    this->games = other.games;
    this->machine_texture = other.machine_texture;
    this->machine_texture_size = other.machine_texture_size;
    this->machine_sprite = other.machine_sprite;
    return *this;
}

Machine::~Machine() {}

void Machine::run(sf::RenderWindow& window, Buttons& buttons) {
    machine_sprite = sf::Sprite(machine_texture);
    machine_sprite.setScale(window.getSize().x / (float)machine_texture_size.x, window.getSize().y / (float)machine_texture_size.y);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (buttons.buttons["enter_game"].isMouseOver(window)) {
                        std::cout << "Entered the game!\n";
                        games[0].run(window, buttons);
                    } else if (buttons.buttons["return_casino"].isMouseOver(window)) {
                        std::cout << "Return to the casino window!\n";
                        return;
                    }
                }
                break;

            default:
                break;
            }
        }
        buttons.buttons["enter_game"].update(window);
        buttons.buttons["return_casino"].update(window);
        window.clear();
        window.draw(machine_sprite);
        window.draw(buttons.buttons["enter_game"]);
        window.draw(buttons.buttons["return_casino"]);
        window.display();
    }
}

std::ostream& operator<<(std::ostream& os, const Machine& m) {
    os << m.name << " is a machine with " << m.games.size() << " games.\n";
    return os;
}
