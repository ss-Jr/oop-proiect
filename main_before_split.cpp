#include <array>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <exception>

class CasinoException : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Casino exception occurred";
    }
};

class MachineException : public CasinoException {
public:
    virtual const char* what() const noexcept override {
        return "Machine exception occurred";
    }
};

class GameException : public CasinoException {
public:
    virtual const char* what() const noexcept override {
        return "Game exception occurred";
    }
};

class ButtonException : public CasinoException {
public:
    virtual const char* what() const noexcept override {
        return "Button exception occurred";
    }
};

class Button : public sf::Drawable {
private:
    sf::Text text;
    sf::RectangleShape rect;
    sf::Color normalColor;
    sf::Color hoverColor;
    std::string text_content;

public:
    Button() {}
    Button(sf::Font& font_, float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, int pos_x = 0, int pos_y = 0, const std::string& text_content_ = "Button")
        : rect(sf::Vector2f(width, height)), normalColor(color), hoverColor(hoverColor), text_content(text_content_)
    {
        text = sf::Text(text_content, font_, 24);
        text.setFillColor(sf::Color::White);
        text.setPosition(pos_x, pos_y);
        rect.setPosition(x, y);
        rect.setFillColor(normalColor);
    }

    Button(const Button& other) : text(other.text), rect(other.rect), normalColor(other.normalColor), hoverColor(other.hoverColor), text_content(other.text_content) {
        std::cout << "Copy constructor Button\n";
    }

    bool isMouseOver(sf::RenderWindow& window) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        return rect.getGlobalBounds().contains(mousePos);
    }

    void update(sf::RenderWindow& window) {
        if (isMouseOver(window)) {
            rect.setFillColor(hoverColor);
        } else {
            rect.setFillColor(normalColor);
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(rect, states);
        target.draw(text);
    }

    Button& operator=(const Button& other) {
        this->text = other.text;
        this->rect = other.rect;
        this->normalColor = other.normalColor;
        this->hoverColor = other.hoverColor;
        this->text_content = other.text_content;
        return *this;
    }

    ~Button() {}

    friend std::ostream& operator<<(std::ostream& os, const Button& b) {
        os << "This button has the text: " << b.text_content << " and the positions: x = " << b.rect.getSize().x << " and y = " << b.rect.getSize().y << ".\n";
        return os;
    }
};

class Buttons {
public:
    std::map<std::string, Button> buttons;

    Buttons() = default;
    explicit Buttons(const std::map<std::string, Button>& m) : buttons(m) {}

    Buttons& operator=(const Buttons& other) {
        this->buttons = other.buttons;
        return *this;
    }

    Buttons(const Buttons& other) : buttons(other.buttons) {
        std::cout << "Copy constructor Buttons\n";
    }

    ~Buttons() {}

    friend std::ostream& operator<<(std::ostream& os, const Buttons& b) {
        os << "This project has " << b.buttons.size() << " buttons.\n";
        return os;
    }
};

class Location {
protected:
    std::string name;
public:
    Location() = default;
    virtual ~Location() = default;
    Location(const std::string& name_) : name(name_) {}
    virtual void run() = 0;
    virtual void run(sf::RenderWindow& window, Buttons& buttons) = 0;
    virtual Location* clone() const = 0;

    static int locationCount;

    Location(const Location& other) : name(other.name) {
        increaseLocationCount();
        std::cout << "Copy constructor Location\n";
    }

    static void increaseLocationCount() {
        locationCount++;
    }

    static int getLocationCount() {
        return locationCount;
    }
};

int Location::locationCount = 0;

class Player {
private:
    std::string name;
    double betting_amount;

public:
    Player(const std::string& n = "Player", double ba = 0) : name(n), betting_amount(ba) {}

    Player(const Player& other) : name(other.name), betting_amount(other.betting_amount) {
        std::cout << "Copy constructor Player\n";
    }

    Player& operator=(const Player& other) {
        this->name = other.name;
        this->betting_amount = other.betting_amount;
        return *this;
    }

    ~Player() {}

    friend std::ostream& operator<<(std::ostream& os, const Player& p) {
        os << "This player has the name: " << p.name << " and has " << p.betting_amount << " credits to bet.\n";
        return os;
    }
};

class Game : public Location {
private:
    static std::vector<double> bet_amounts;
    sf::Texture game_texture;
    sf::Vector2u game_texture_size;
    sf::Sprite game_sprite;
    std::array<char, 3> reels;
    std::array<char, 4> symbols = { 'a', 'b', 'c', 'd' };

public:
    Game(const std::string& name_ = "Slot Game") : Location(name_) {
        if (!game_texture.loadFromFile("game.jpg")) {
            std::cout << "Could not load game textures.\n";
        } else {
            game_texture_size = game_texture.getSize();
            game_sprite = sf::Sprite(game_texture);
        }
    }

    Location* clone() const override {
        return new Game(*this);
    }

    Game(const Game& other) : Location(other), game_texture(other.game_texture), game_texture_size(other.game_texture_size), game_sprite(other.game_sprite), reels(other.reels), symbols(other.symbols) {
        std::cout << "Copy constructor Game\n";
    }

    Game& operator=(const Game& other) {
        Location::operator=(other);
        this->game_texture = other.game_texture;
        this->game_texture_size = other.game_texture_size;
        this->game_sprite = other.game_sprite;
        this->reels = other.reels;
        this->symbols = other.symbols;
        return *this;
    }

    ~Game() {}

    void spin() {
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

    bool checkWin() {
        return (reels[0] == reels[1] && reels[1] == reels[2]);
    }

    bool displayResult() {
        int reels_size = reels.size();
        std::cout << "Reels: ";
        for (int i = 0; i < reels_size; ++i)
            std::cout << reels[i] << ' ';
        std::cout << '\n';
        return checkWin();
    }

    void run() override {}

    void run(sf::RenderWindow& window, Buttons& buttons) override {
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

    friend std::ostream& operator<<(std::ostream& os, const Game& g) {
        os << g.name << " has " << g.bet_amounts.size() << " bet amounts: ";
        for (auto x : bet_amounts) {
            os << x << ' ';
        }
        os << ", " << g.reels.size() << " reels and " << g.symbols.size() << " symbols: ";
        for (auto symbol : g.symbols) {
            os << symbol << ' ';
        }
        os << '\n';
        return os;
    }
};

std::vector<double> Game::bet_amounts;

class Machine : public Location {
private:
    std::vector<Game> games;
    sf::Texture machine_texture;
    sf::Vector2u machine_texture_size;
    sf::Sprite machine_sprite;

public:
    Machine(const std::string& name_ = "Slot Machine") : Location(name_), games(std::vector<Game>(1, Game("Slot Game"))) {
        if (!machine_texture.loadFromFile("games.jpg")) {
            std::cout << "Could not load slot machine textures.\n";
        } else {
            machine_texture_size = machine_texture.getSize();
        }
    }

    Location* clone() const override {
        return new Machine(*this);
    }

    Machine(const Machine& other) : Location(other), games(other.games), machine_texture(other.machine_texture), machine_texture_size(other.machine_texture_size), machine_sprite(other.machine_sprite) {
        std::cout << "Copy constructor Machine\n";
    }

    Machine& operator=(const Machine& other) {
        Location::operator=(other);
        this->games = other.games;
        this->machine_texture = other.machine_texture;
        this->machine_texture_size = other.machine_texture_size;
        this->machine_sprite = other.machine_sprite;
        return *this;
    }

    ~Machine() {}

    void run() override {}

    void run(sf::RenderWindow& window, Buttons& buttons) override {
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

    friend std::ostream& operator<<(std::ostream& os, const Machine& m) {
        os << m.name << " is a machine with " << m.games.size() << " games.\n";
        return os;
    }
};

class Casino : public Location {
private:
    std::vector<Machine> machines;
    std::vector<Player> players;
    sf::Texture casino_texture;
    sf::Vector2u casino_texture_size;
    sf::Sprite casino_sprite;

public:
    Casino(const std::string& name_ = "Casino") : Location(name_), machines(std::vector<Machine>(1, Machine("Slot Machine"))), players(std::vector<Player>(1, Player("Player-1", 2000))) {
        if (!casino_texture.loadFromFile("machine.jpg")) {
            std::cout << "Could not load casino textures.\n";
        } else {
            casino_texture_size = casino_texture.getSize();
        }
    }

    Location* clone() const override {
        return new Casino(*this);
    }

    Casino(const Casino& other) : Location(other), machines(other.machines), players(other.players), casino_texture(other.casino_texture), casino_texture_size(other.casino_texture_size), casino_sprite(other.casino_sprite) {
        std::cout << "Copy constructor Casino\n";
    }

    Casino& operator=(const Casino& other) {
        Location::operator=(other);
        this->machines = other.machines;
        this->players = other.players;
        this->casino_texture = other.casino_texture;
        this->casino_texture_size = other.casino_texture_size;
        this->casino_sprite = other.casino_sprite;
        return *this;
    }

    ~Casino() {}

    void run() override {}

    void run(sf::RenderWindow& window, Buttons& buttons) override {
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

    friend std::ostream& operator<<(std::ostream& os, const Casino& c) {
        os << c.name << " is a casino with " << c.machines.size() << " machines and " << c.players.size() << " players.\n";
        return os;
    }
};

class Application : public Location {
private:
    std::vector<Casino> casinos;
    sf::RenderWindow window;
    sf::Texture app_texture;
    sf::Vector2u app_texture_size;
    sf::Sprite app_sprite;

public:
    Application(const std::string& name_ = "Pacanea") : Location(name_), casinos(std::vector<Casino>(1, Casino("Casino"))), window(sf::VideoMode(800, 600), name_, sf::Style::Default) {
        if (!app_texture.loadFromFile("app.jpeg")) {
            std::cout << "Could not load app textures.\n";
        } else {
            app_texture_size = app_texture.getSize();
        }
    }

    Location* clone() const override {
        return new Application(*this);
    }

    ~Application() {}

    Application& operator=(const Application& other) {
        Location::operator=(other);
        this->app_texture = other.app_texture;
        this->app_texture_size = other.app_texture_size;
        this->app_sprite = other.app_sprite;
        return *this;
    }

    Application(const Application& other) : Location(other), casinos(other.casinos), app_texture(other.app_texture), app_texture_size(other.app_texture_size), app_sprite(other.app_sprite) {
        std::cout << "Copy constructor Application\n";
    }

    void run(sf::RenderWindow& window, Buttons& buttons) override {}

    void run() override {
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

    friend std::ostream& operator<<(std::ostream& os, const Application& a) {
        os << a.name << " is an application with " << a.casinos.size() << " casinos.\n";
        return os;
    }
};

class Project {
private:
    std::unique_ptr<Application> ptr;
public:
    Project(std::unique_ptr<Application> ptr_) : ptr(std::move(ptr_)) {}

    void start() const {
        ptr->run();
    }

    Project& operator=(Project other) {
        swap(*this, other);
        return *this;
    }

    friend void swap(Project& p1, Project& p2) {
        std::swap(p1.ptr, p2.ptr);
    }
};

int main() {
    try {
        auto app = std::make_unique<Application>("Pacanea");
        Project project(std::move(app));
        project.start();
    } catch (const CasinoException& e) {
        std::cerr << "Exception caught in main(): " << e.what() << '\n';
    }
    return 0;
}
