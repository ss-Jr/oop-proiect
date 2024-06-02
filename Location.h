#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Buttons.h"

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

    Location& operator=(const Location& other);

    static int locationCount;

    Location(const Location& other);

    static void increaseLocationCount();

    static int getLocationCount();
};

#endif // LOCATION_H
