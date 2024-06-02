#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

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

#endif // BUTTON_H
