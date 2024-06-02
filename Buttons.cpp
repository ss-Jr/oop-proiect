#include "Buttons.h"

Buttons& Buttons::operator=(const Buttons& other) {
    this->buttons = other.buttons;
    return *this;
}

Buttons::Buttons(const Buttons& other) : buttons(other.buttons) {
    std::cout << "Copy constructor Buttons\n";
}

Buttons::~Buttons() {}

std::ostream& operator<<(std::ostream& os, const Buttons& b) {
    os << "This project has " << b.buttons.size() << " buttons.\n";
    return os;
}
