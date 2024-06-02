#include "Location.h"

int Location::locationCount = 0;

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
