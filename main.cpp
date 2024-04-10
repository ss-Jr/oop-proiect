#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable
{
private:
  sf::Text text;
  sf::Font font;
  sf::RectangleShape rect;
  sf::Color normalColor;
  sf::Color hoverColor;

  std::string text_content;

public:
  Button(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, int pos_x = 0, int pos_y = 0, std::string text_content_ = "Button")
      : rect(sf::Vector2f(width, height)), normalColor(color), hoverColor(hoverColor), text_content(text_content_)
  {
    if (!font.loadFromFile("Arial.ttf"))
    {
      std::cout << "Could not load font.\n";
      // return EXIT_FAILURE;
    }
    else
    {
      text = sf::Text(text_content, font, 24);
      text.setFillColor(sf::Color::White);
      text.setPosition(pos_x, pos_y);
    }
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
};

class Player
{
private:
  std::string name;
  double betting_amount;

public:
  Player(std::string n = "Player", double ba = 0) : name(n), betting_amount(ba) {}
  friend std::ostream &operator<<(std::ostream &os, const Player &p)
  {
    os << p.name;
    return os;
  }
};

class Game
{
private:
  std::string name;
  std::vector<double> bet_amounts;
  // const std::vector<std::string> choices = {"spin", "double", "cashout", "add_amount"};
  bool double_choice;

  sf::Texture g_texture;
  sf::Vector2u g_texture_size;
  sf::Sprite g_sprite;
  Button button;

  const int numReels = 3;
  char reels[3];
  char symbols[4] = {'a', 'b', 'c', 'd'};
  const int numSymbols = 4;

public:
  Game(std::string name_ = "Slot Game") : name(name_), button(Button(100, 100, 200, 50, sf::Color::Blue, sf::Color::Cyan))
  {
    if (!g_texture.loadFromFile("slot.png"))
    {
      std::cout << "Could not load game textures.\n";
      // return EXIT_FAILURE;
    }
    else
    {
      g_texture_size = g_texture.getSize();
      g_sprite = sf::Sprite(g_texture);
    }
  }

  Game operator=(const Game &other)
  {
    this->name = other.name;
    this->bet_amounts = other.bet_amounts;

    return *this;
  }

  ~Game() {}

  /*friend void RandomCardChoice (Game& g) {
    g.double_choice = rand() & 1;
  }*/

  void spin()
  {
    for (int i = 0; i < numReels; ++i)
      reels[i] = symbols[rand() % numSymbols];
  }

  bool checkWin()
  {
    return (reels[0] == reels[1] && reels[1] == reels[2]);
  }

  void displayResult()
  {
    std::cout << "Reels: ";
    for (int i = 0; i < numReels; ++i)
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

  void run(sf::RenderWindow &window, Button &button, Button &back_button)
  {
    g_sprite = sf::Sprite(g_texture);
    g_sprite.setScale(window.getSize().x / (float)g_texture_size.x, window.getSize().y / (float)g_texture_size.y);
    bool was = false;
    while (window.isOpen())
    {
      sf::Event event;
      bool is = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space));
      bool is2 = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V));
      if (!was && is)
      {
        std::cout << "Spin!\n";
        spin();
        while (checkWin() && !is2)
          spin();
        displayResult();
      }
      was = is;
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
            if (button.isMouseOver(window))
            {
              std::cout << "Spin!\n";
              spin();
              displayResult();
              // machines[0].run();
            }
            else if (back_button.isMouseOver(window))
            {
              std::cout << "Return to the machine window!\n";
              return;
            }

          default:
            break;
          }
        }
        button.update(window);
        window.clear();
        window.draw(g_sprite);
        window.draw(button);
        window.draw(back_button);
        // window.draw(button.text);
        window.display();
      }
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const Game &g)
  {
    os << g.name;
    return os;
  }
};

class Machine
{
private:
  std::string name;
  std::vector<Game> games;

  sf::Texture m_texture;
  sf::Vector2u m_texture_size;
  sf::Sprite m_sprite;
  Button button;

public:
  Machine(std::string name_ = "Slot Machine") : name(name_), games(std::vector<Game>(1, Game("Slot Game"))), button(Button(100, 100, 200, 50, sf::Color::Blue, sf::Color::Cyan))
  {
    if (!m_texture.loadFromFile("machine.png"))
    {
      std::cout << "Could not load slot machine textures.\n";
      // return EXIT_FAILURE;
    }
    else
    {
      m_texture_size = m_texture.getSize();
    }
  }

  Machine operator=(const Machine &other)
  {
    this->name = other.name;
    this->games = other.games;

    return *this;
  }

  ~Machine() {}

  void run(sf::RenderWindow &window, Button &button, Button &back_button)
  {
    m_sprite = sf::Sprite(m_texture);
    m_sprite.setScale(window.getSize().x / (float)m_texture_size.x, window.getSize().y / (float)m_texture_size.y);
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
            if (button.isMouseOver(window))
            {
              std::cout << "Button Clicked in machine!\n";
              games[0].run(window, button, back_button);
            }
            else if (back_button.isMouseOver(window))
            {
              std::cout << "Return to the casino window!\n";
              return;
            }

          default:
            break;
          }
        }
        button.update(window);
        window.clear();
        window.draw(m_sprite);
        window.draw(button);
        window.draw(back_button);
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
  sf::Texture c_texture;
  sf::Vector2u c_texture_size;
  sf::Sprite c_sprite;
  Button button;

public:
  Casino(std::string name_ = "Casino") : /*window(window_),*/ name(name_), machines(std::vector<Machine>(1, Machine("Slot Machine"))), players(std::vector<Player>(1, Player("Player-1", 2000))), button(Button(100, 100, 200, 50, sf::Color::Blue, sf::Color::Cyan))
  {
    if (!c_texture.loadFromFile("app.png"))
    {
      std::cout << "Could not load casino textures.\n";
      // return EXIT_FAILURE;
    }
    else
    {
      c_texture_size = c_texture.getSize();
    }
  }

  /*Casino operator=(const Casino& other) {
    this->name = other.name;
    //this->machines = other.machines;
    this->players = other.players;

    return *this;
  }*/

  ~Casino() {}

  void run(sf::RenderWindow &window, Button &button, Button &back_button)
  {
    c_sprite.setTexture(c_texture);
    c_sprite.setScale(window.getSize().x / (float)c_texture_size.x, window.getSize().y / (float)c_texture_size.y);
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
            if (button.isMouseOver(window))
            {
              std::cout << "Button Clicked in casino!\n";
              machines[0].run(window, button, back_button);
            }
            else if (back_button.isMouseOver(window))
            {
              std::cout << "Return to the app window!\n";
              return;
            }
          }

        default:
          break;
        }
      }
      button.update(window);
      back_button.update(window);
      window.clear();
      window.draw(c_sprite);
      window.draw(button);
      window.draw(back_button);
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
  sf::Texture a_texture;
  sf::Vector2u a_texture_size;
  sf::Sprite a_sprite;
  Button button, back_button;

public:
  Application(std::string name_ = "Pacanea") : window(sf::VideoMode(800, 600), name, sf::Style::Default), name(name_), casinos(std::vector<Casino>(1, Casino("Casino"))), button(Button(100, 100, 200, 50, sf::Color::Blue, sf::Color::Cyan, 120, 120)), back_button(Button(10, 10, 200, 50, sf::Color::Blue, sf::Color::Cyan, 30, 30, "Go back"))
  {
    if (!a_texture.loadFromFile("app.png"))
    {
      std::cout << "Could not load app textures.\n";
      // return EXIT_FAILURE;
    }
    else
    {
      a_texture_size = a_texture.getSize();
    }
  }

  void run()
  {
    a_sprite.setTexture(a_texture);
    a_sprite.setScale(window.getSize().x / (float)a_texture_size.x, window.getSize().y / (float)a_texture_size.y);
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
          if (event.mouseButton.button == sf::Mouse::Left && button.isMouseOver(window))
          {
            std::cout << "Button Clicked!" << std::endl;
            casinos[0].run(window, button, back_button);
          }

        default:
          break;
        }
      }
      // button.update(window);
      window.clear();
      window.draw(a_sprite);
      window.draw(button);
      // window.draw(button.text);
      window.display();
    }
  }

  ~Application() {}

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
