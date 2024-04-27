#include <array>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable
{
private:
  sf::Text text;
  sf::RectangleShape rect;
  sf::Color normalColor;
  sf::Color hoverColor;

  std::string text_content;

public:
  Button() {}
  //Button(sf::Font& font_) : font(font_) {}
  Button(sf::Font& font_, float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, int pos_x = 0, int pos_y = 0, const std::string text_content_ = "Button")
      : rect(sf::Vector2f(width, height)), normalColor(color), hoverColor(hoverColor), text_content(text_content_)
  {
    /*if (!font.loadFromFile("Arial.ttf"))
    {
      std::cout << "Could not load font.\n";
      // return EXIT_FAILURE;
    }
    else
    {*/
      text = sf::Text(text_content, font_, 24);
      text.setFillColor(sf::Color::White);
      text.setPosition(pos_x, pos_y);
    //}
    rect.setPosition(x, y);
    rect.setFillColor(normalColor);
  }

  bool isMouseOver(sf::RenderWindow &window)
  {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    return rect.getGlobalBounds().contains(mousePos);
  }

  void update(sf::RenderWindow &window)
  {
    if (isMouseOver(window))
    {
      rect.setFillColor(hoverColor);
    }
    else
    {
      rect.setFillColor(normalColor);
    }
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override
  {
    target.draw(rect, states);
    target.draw(text);
  }
  /*void draw(sf::RenderWindow& window) {
      window.draw(rect);
  }*/
  friend std::ostream &operator<<(std::ostream &os, const Button &b)
  {
    os << b.text_content;
    return os;
  }
};

class Buttons {
public:
  std::map<std::string, Button> buttons;

  Buttons(const std::map<std::string, Button> m) : buttons(m) {}
  friend std::ostream &operator<<(std::ostream &os, const Buttons &b)
  {
    os << "This project has " << b.buttons.size() << " buttons\n";
    return os;
  }

  
};

class Player
{
private:
  std::string name;
  double betting_amount;

public:
  Player(const std::string n = "Player", double ba = 0) : name(n), betting_amount(ba) {}
  friend std::ostream &operator<<(std::ostream &os, const Player &p)
  {
    os << p.name << ' ' << p.betting_amount;
    return os;
  }
};

class Game
{
private:
  std::string name;
  static std::vector<double> bet_amounts;
  // const std::vector<std::string> choices = {"spin", "double", "cashout", "add_amount"};
  //bool double_choice;

  sf::Texture game_texture;
  sf::Vector2u game_texture_size;
  sf::Sprite game_sprite;

  std::array<char, 3> reels;
  std::array<char, 4> symbols = {'a', 'b', 'c', 'd'};

public:
  Game(const std::string name_ = "Slot Game") : name(name_)
  {
    if (!game_texture.loadFromFile("game.jpg"))
    {
      std::cout << "Could not load game textures.\n";
      // return EXIT_FAILURE;
    }
    else
    {
      game_texture_size = game_texture.getSize();
      game_sprite = sf::Sprite(game_texture);
    }
  }

  Game &operator=(const Game &other)
  {
    this->name = other.name;
    this->bet_amounts = other.bet_amounts;
    this->game_texture = other.game_texture;
    this->game_texture_size = other.game_texture_size;
    this->game_sprite = other.game_sprite;

    return *this;
  }

  ~Game() {}

  /*friend void RandomCardChoice (Game& g) {
    g.double_choice = rand() & 1;
  }*/

  void spin()
  {
    int chance = rand() % 100;
    int reels_size = reels.size();
    for (int i = 0; i < reels_size; ++i)
      reels[i] = symbols[rand() % symbols.size()];
    if(chance < 80) {
      while(!checkWin()) {
        for (int i = 0; i < reels_size; ++i)
          reels[i] = symbols[rand() % symbols.size()];
      }
    } else {
      while(checkWin()) {
        for (int i = 0; i < reels_size; ++i)
          reels[i] = symbols[rand() % symbols.size()];
      }
    }
  }

  bool checkWin()
  {
    return (reels[0] == reels[1] && reels[1] == reels[2]);
  }

  void displayResult()
  {
    std::cout << "Reels: ";
    for (int i = 0; i < reels.size(); ++i)
      std::cout << reels[i] << ' ';
    std::cout << '\n';
    if (checkWin())
    {
      std::cout << "Congratulations! You won!\n";
    }
    else
    {
      std::cout << "Better luck next time!\n";
    }
  }

  void run(sf::RenderWindow &window, Buttons &buttons)
  {
    game_sprite = sf::Sprite(game_texture);
    game_sprite.setScale(window.getSize().x / (float)game_texture_size.x, window.getSize().y / (float)game_texture_size.y);
    while (window.isOpen())
    {
      sf::Event event;
      while (window.pollEvent(event))
      {
        switch (event.type)
        {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::MouseButtonPressed:
          if (event.mouseButton.button == sf::Mouse::Left)
          {
            if (buttons.buttons["spin"].isMouseOver(window))
            {
              std::cout << "Spin!\n";
              spin();
              displayResult();
              // machines[0].run();
            }
            else if (buttons.buttons["return_machine"].isMouseOver(window))
            {
              std::cout << "Return to the machine window!\n";
              return;
            }

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
        // window.draw(button.text);
        window.display();
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const Game &g)
  {
    os << g.name << "\nBet amounts:\n";
    for(auto x : bet_amounts) {
      os << x << ' ';
    }
    return os;
  }
};

class Machine
{
private:
  std::string name;
  std::vector<Game> games;

  sf::Texture machine_texture;
  sf::Vector2u machine_texture_size;
  sf::Sprite machine_sprite;
  //Button button;

public:
  Machine(const std::string name_ = "Slot Machine") : name(name_), games(std::vector<Game>(1, Game("Slot Game"))) {
    if (!machine_texture.loadFromFile("games.jpg"))
    {
      std::cout << "Could not load slot machine textures.\n";
      // return EXIT_FAILURE;
    }
    else
    {
      machine_texture_size = machine_texture.getSize();
    }
  }

  Machine &operator=(const Machine &other)
  {
    this->name = other.name;
    this->games = other.games;
    this->machine_texture = other.machine_texture;
    this->machine_texture_size = other.machine_texture_size;
    this->machine_sprite = other.machine_sprite;

    return *this;
  }

  ~Machine() {}

  void run(sf::RenderWindow &window, Buttons &buttons)
  {
    machine_sprite = sf::Sprite(machine_texture);
    machine_sprite.setScale(window.getSize().x / (float)machine_texture_size.x, window.getSize().y / (float)machine_texture_size.y);
    while (window.isOpen())
    {
      sf::Event event;
      while (window.pollEvent(event))
      {
        switch (event.type)
        {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::MouseButtonPressed:
          if (event.mouseButton.button == sf::Mouse::Left)
          {
            if (buttons.buttons["enter_game"].isMouseOver(window))
            {
              std::cout << "Entered the game!\n";
              games[0].run(window, buttons);
            }
            else if (buttons.buttons["return_casino"].isMouseOver(window))
            {
              std::cout << "Return to the casino window!\n";
              return;
            }

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
        // window.draw(button.text);
        window.display();
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const Machine &m)
  {
    os << m.name;
    return os;
  }
};

class Casino
{
private:
  std::string name;
  std::vector<Machine> machines;
  std::vector<Player> players;

  // sf::RenderWindow& window;
  sf::Texture casino_texture;
  sf::Vector2u casino_texture_size;
  sf::Sprite casino_sprite;
  //Button button;

public:
  Casino(const std::string name_ = "Casino") : /*window(window_),*/ name(name_), machines(std::vector<Machine>(1, Machine("Slot Machine"))), players(std::vector<Player>(1, Player("Player-1", 2000)))
  {
    if (!casino_texture.loadFromFile("machine.jpg"))
    {
      std::cout << "Could not load casino textures.\n";
      // return EXIT_FAILURE;
    }
    else
    {
      casino_texture_size = casino_texture.getSize();
    }
  }

  Casino &operator=(const Casino& other) {
    this->name = other.name;
    this->machines = other.machines;
    this->players = other.players;
    this->casino_texture = other.casino_texture;
    this->casino_texture_size = other.casino_texture_size;
    this->casino_sprite = other.casino_sprite;

    return *this;
  }

  ~Casino() {}

  void run(sf::RenderWindow &window, Buttons &buttons)
  {
    casino_sprite.setTexture(casino_texture);
    casino_sprite.setScale(window.getSize().x / (float)casino_texture_size.x, window.getSize().y / (float)casino_texture_size.y);
    while (window.isOpen())
    {
      sf::Event event;
      while (window.pollEvent(event))
      {
        switch (event.type)
        {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::MouseButtonPressed:
          if (event.mouseButton.button == sf::Mouse::Left)
          {
            if (buttons.buttons["enter_machine"].isMouseOver(window))
            {
              std::cout << "Entered in machine!\n";
              machines[0].run(window, buttons);
            }
            else if (buttons.buttons["return_app"].isMouseOver(window))
            {
              std::cout << "Return to the app window!\n";
              return;
            }
          }

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
      // window.draw(button.text);
      window.display();
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const Casino &c)
  {
    os << c.name;
    return os;
  }
};

class Application
{
private:
  std::string name;
  std::vector<Casino> casinos;

  sf::RenderWindow window;
  sf::Texture app_texture;
  sf::Vector2u app_texture_size;
  sf::Sprite app_sprite;
  //Button button, back_button;
  // Buttons buttons;

public:
  Application(const std::string name_ = "Pacanea") : window(sf::VideoMode(800, 600), name_, sf::Style::Default), name(name_), casinos(std::vector<Casino>(1, Casino("Casino"))) {
    if (!app_texture.loadFromFile("app.jpeg"))
    {
      std::cout << "Could not load app textures.\n";
      // return EXIT_FAILURE;
    }
    else
    {
      app_texture_size = app_texture.getSize();
    }
  }

  void run()
  {
    sf::Font font;
    if (!font.loadFromFile("Arial.ttf"))
    {
      std::cout << "Could not load font.\n";
      return;
      // return EXIT_FAILURE;
    }
    Buttons buttons = Buttons({
    {"exit_app", Button(font, 10, 10, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 30, 30, "EXIT")},
    {"enter_casino", Button(font, 100, 100, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 120, 120, "Enter Casino")},
    {"return_app", Button(font, 10, 10, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 30, 30, "Return to App")},
    {"enter_machine", Button(font, 100, 100, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 120, 120, "Enter Machine")},
    {"return_casino", Button(font, 10, 10, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 30, 30, "Return to Casino")},
    {"enter_game", Button(font, 100, 100, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 120, 120, "Enter Game")},
    {"return_machine", Button(font, 10, 10, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 30, 30, "Return to machine")},
    {"spin", Button(font, 100, 100, 220, 50, sf::Color{ 0x0036B1FF }, sf::Color{ 0x00339AFF }, 120, 120, "Spin")},
    });
    app_sprite.setTexture(app_texture);
    app_sprite.setScale(window.getSize().x / (float)app_texture_size.x, window.getSize().y / (float)app_texture_size.y);
    // window.create(sf::VideoMode(800, 600), name, sf::Style::Default);
    window.setFramerateLimit(60);
    while (window.isOpen())
    {
      sf::Event event;
      while (window.pollEvent(event))
      {
        switch (event.type)
        {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::MouseButtonPressed:
          if (event.mouseButton.button == sf::Mouse::Left)
          {
            if (buttons.buttons["enter_casino"].isMouseOver(window))
            {
              std::cout << "Entered in casino!\n";
              casinos[0].run(window, buttons);
            }
            else if (buttons.buttons["exit_app"].isMouseOver(window))
            {
              std::cout << "Closed the application!\n";
              return;
            }
          }

        default:
          break;
        }
      }
      // button.update(window);
      buttons.buttons["enter_casino"].update(window);
      buttons.buttons["exit_app"].update(window);
      window.clear();
      window.draw(app_sprite);
      window.draw(buttons.buttons["enter_casino"]);
      window.draw(buttons.buttons["exit_app"]);
      // window.draw(buttons.buttons["enter_casino"].text);
      window.display();
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const Application &a)
  {
    os << a.name;
    return os;
  }
};

int main()
{
  Application app("Pacanea");
  app.run();
  return 0;
}
