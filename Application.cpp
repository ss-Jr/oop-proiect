#include "Application.h"
#include "ButtonException.h"
#include <iostream>

Application::Application(const std::string& name_) : Location(name_), casinos(std::vector<Casino>(1, Casino("Casino"))), window(sf::VideoMode(800, 600), name_, sf::Style::Default) {
    if (!app_texture.loadFromFile("app.jpeg")) {
        std::cout << "Could not load app textures.\n";
    } else {
        app_texture_size = app_texture.getSize();
    }
}

Location* Application::clone() const {
    return new Application(*this);
}

Application::~Application() {}

Application& Application::operator=(const Application& other) {
    Location::operator=(other);
    this->app_texture = other.app_texture;
    this->app_texture_size = other.app_texture_size;
    this->app_sprite = other.app_sprite;
    return *this;
}

Application::Application(const Application& other) : Location(other), casinos(other.casinos), app_texture(other.app_texture), app_texture_size(other.app_texture_size), app_sprite(other.app_sprite) {
    std::cout << "Copy constructor Application\n";
}

void Application::run() {
    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        std::cout << "Could not load font.\n";
        return;
    }
    Buttons buttons;
    try {
        buttons = Buttons({
            {"exit_app", Button(font, 10, 10, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 30, 30, "EXIT")},
            {"enter_casino", Button(font, 100, 100, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 120, 120, "Enter Casino")},
            {"return_app", Button(font, 10, 10, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 30, 30, "Return to App")},
            {"enter_machine", Button(font, 100, 100, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 120, 120, "Enter Machine")},
            {"return_casino", Button(font, 10, 10, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 30, 30, "Return to Casino")},
            {"enter_game", Button(font, 100, 100, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 120, 120, "Enter Game")},
            {"return_machine", Button(font, 10, 10, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 30, 30, "Return to machine")},
            {"spin", Button(font, 100, 100, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 120, 120, "Spin")}
        });
    } catch (const ButtonException& e) {
        std::cerr << "Exception caught while trying to construct the buttons: " << e.what() << '\n';
        return;
    }
    app_sprite.setTexture(app_texture);
    app_sprite.setScale(window.getSize().x / (float)app_texture_size.x, window.getSize().y / (float)app_texture_size.y);
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (buttons.buttons["enter_casino"].isMouseOver(window)) {
                        std::cout << "Entered in casino!\n";
                        casinos[0].run(window, buttons);
                    } else if (buttons.buttons["exit_app"].isMouseOver(window)) {
                        std::cout << "Closed the application!\n";
                        return;
                    }
                }
                break;

            default:
                break;
            }
        }
        buttons.buttons["enter_casino"].update(window);
        buttons.buttons["exit_app"].update(window);
        window.clear();
        window.draw(app_sprite);
        window.draw(buttons.buttons["enter_casino"]);
        window.draw(buttons.buttons["exit_app"]);
        window.display();
    }
}

std::ostream& operator<<(std::ostream& os, const Application& a) {
    os << a.name << " is an application with " << a.casinos.size() << " casinos.\n";
    return os;
}
