#include "Game.h"
#include <iostream>
#include <SFML/Audio.hpp>

std::vector<double> Game::bet_amounts;

Game::Game(const std::string& name_) : Location(name_) {
    if (!game_texture.loadFromFile("game.jpg")) {
        std::cout << "Could not load game textures.\n";
    } else {
        game_texture_size = game_texture.getSize();
        game_sprite = sf::Sprite(game_texture);
    }
}

Location* Game::clone() const {
    return new Game(*this);
}

Game::Game(const Game& other) : Location(other), game_texture(other.game_texture), game_texture_size(other.game_texture_size), game_sprite(other.game_sprite), reels(other.reels), symbols(other.symbols) {
    std::cout << "Copy constructor Game\n";
}

Game& Game::operator=(const Game& other) {
    if (this == &other)
        return *this;
    Location::operator=(other);
    this->game_texture = other.game_texture;
    this->game_texture_size = other.game_texture_size;
    this->game_sprite = other.game_sprite;
    this->reels = other.reels;
    this->symbols = other.symbols;
    return *this;
}

Game::~Game() {}

void Game::spin() {
    int chance = rand() % 100;
    int reels_size = reels.size();
    for (int i = 0; i < reels_size; ++i)
        reels[i] = symbols[rand() % symbols.size()];
    if (chance < 80) {
        while (!checkWin()) {
            for (int i = 0; i < reels_size; ++i)
                reels[i] = symbols[rand() % symbols.size()];
        }
    } else {
        while (checkWin()) {
            for (int i = 0; i < reels_size; ++i)
                reels[i] = symbols[rand() % symbols.size()];
        }
    }
}

bool Game::checkWin() {
    return (reels[0] == reels[1] && reels[1] == reels[2]);
}

bool Game::displayResult() {
    int reels_size = reels.size();
    std::cout << "Reels: ";
    for (int i = 0; i < reels_size; ++i)
        std::cout << reels[i] << ' ';
    std::cout << '\n';
    return checkWin();
}

void Game::run(sf::RenderWindow& window, Buttons& buttons) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("slot_win.mp3")) {
        std::cerr << "Failed to load audio file\n";
        return;
    }
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.setVolume(25);

    float durationInSeconds = 1.5f;

    game_sprite = sf::Sprite(game_texture);
    game_sprite.setScale(window.getSize().x / (float)game_texture_size.x, window.getSize().y / (float)game_texture_size.y);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (buttons.buttons["spin"].isMouseOver(window)) {
                        std::cout << "Spin!\n";
                        spin();
                        if (displayResult()) {
                            std::cout << "Congratulations! You won!\n";
                            if (Game* gamePtr = dynamic_cast<Game*>(this)) {
                                gamePtr->getLocationCount();
                                sound.play();
                                sf::Clock clock;
                                while (clock.getElapsedTime().asSeconds() < durationInSeconds) {}
                                sound.stop();
                            }
                        } else {
                            std::cout << "Better luck next time!\n";
                        }
                    } else if (buttons.buttons["return_machine"].isMouseOver(window)) {
                        std::cout << "Return to the machine window!\n";
                        return;
                    }
                }
                break;

            default:
                break;
            }
        }
        buttons.buttons["spin"].update(window);
        buttons.buttons["return_machine"].update(window);
        window.clear();
        window.draw(game_sprite);
        window.draw(buttons.buttons["spin"]);
        window.draw(buttons.buttons["return_machine"]);
        window.display();
    }
}

std::ostream& operator<<(std::ostream& os, const Game& g) {
    os << g.name << " has " << g.bet_amounts.size() << " bet amounts: ";
    for (auto x : Game::bet_amounts) {
        os << x << ' ';
    }
    os << ", " << g.reels.size() << " reels and " << g.symbols.size() << " symbols: ";
    for (auto symbol : g.symbols) {
        os << symbol << ' ';
    }
    os << '\n';
    return os;
}
