#include "Location.h"

int Location::locationCount = 0;

Location& Location::operator=(const Location& other) {
    if (this == &other)
      return *this;
    this->name = other.name;
    return *this;
}

Location::Location(const Location& other) : name(other.name) {
    increaseLocationCount();
    std::cout << "Copy constructor Location\n";
}

void Location::increaseLocationCount() {
    locationCount++;
}

int Location::getLocationCount() {
    return locationCount;
}
