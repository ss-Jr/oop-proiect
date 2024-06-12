#include "PlayerDependent.h"

std::istream& PlayerDependent::read(std::istream& is) {
  std::cout << "\nIntrodu numele player-ului: ";
  getline(is, name);
  std::cout << "\nIntrodu balanta player-ului: ";
  is >> balance;
  is.get();
  return is;
}

std::ostream& PlayerDependent::write(std::ostream& os) {
  os << "\nPlayer-ul " << name << " a ramas cu balanta: " << balance;
  return os;
}
