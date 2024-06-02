#ifndef BUTTONS_H
#define BUTTONS_H

#include "Button.h"
#include <map>

class Buttons {
public:
    std::map<std::string, Button> buttons;

    Buttons() = default;
    explicit Buttons(const std::map<std::string, Button>& m) : buttons(m) {}

    Buttons& operator=(const Buttons& other);
    Buttons(const Buttons& other);
    ~Buttons();

    friend std::ostream& operator<<(std::ostream& os, const Buttons& b);
};

#endif // BUTTONS_H
