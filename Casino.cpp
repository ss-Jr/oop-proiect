#include "Casino.h"
#include <iostream>

Casino::Casino(const std::string& name_) : Location(name_), machines(std::vector<Machine>(1, Machine("Slot Machine"))), players(std::vector<Player>(1, Player("Player-1", 2000))) {
    if (!casino_texture.loadFromFile("machine.jpg")) {
        std::cout << "Could not load casino textures.\n";
    } else {
        casino_texture_size = casino_texture.getSize();
    }
}

Location* Casino::clone() const {
    return new Casino(*this);
}

Casino::Casino(const Casino& other) : Location(other), machines(other.machines), players(other.players), casino_texture(other.casino_texture), casino_texture_size(other.casino_texture_size), casino_sprite(other.casino_sprite) {
    std::cout << "Copy constructor Casino\n";
}

Casino& Casino::operator=(const Casino& other) {
    Location::operator=(other);
    this->machines = other.machines;
    this->players = other.players;
    this->casino_texture = other.casino_texture;
    this->casino_texture_size = other.casino_texture_size;
    this->casino_sprite = other.casino_sprite;
    return *this;
}

Casino::~Casino() {}

void Casino::run(sf::RenderWindow& window, Buttons& buttons) {
    casino_sprite.setTexture(casino_texture);
    casino_sprite.setScale(window.getSize().x / (float)casino_texture_size.x, window.getSize().y / (float)casino_texture_size.y);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (buttons.buttons["enter_machine"].isMouseOver(window)) {
                        std::cout << "Entered in machine!\n";
                        machines[0].run(window, buttons);
                    } else if (buttons.buttons["return_app"].isMouseOver(window)) {
                        std::cout << "Return to the app window!\n";
                        return;
                    }
                }
                break;

            default:
                break;
            }
        }
        buttons.buttons["enter_machine"].update(window);
        buttons.buttons["return_app"].update(window);
        window.clear();
        window.draw(casino_sprite);
        window.draw(buttons.buttons["enter_machine"]);
        window.draw(buttons.buttons["return_app"]);
        window.display();
    }
}

std::ostream& operator<<(std::ostream& os, const Casino& c) {
    os << c.name << " is a casino with " << c.machines.size() << " machines and " << c.players.size() << " players.\n";
    return os;
}
