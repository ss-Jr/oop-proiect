#include "PlayerNedependent.h"

std::istream& PlayerNedependent::read(std::istream& is) {
  std::cout << "\nIntrodu numele player-ului: ";
  getline(is, name);
  std::cout << "\nIntrodu balanta player-ului: ";
  is >> balance;
  is.get();
  return is;
}

std::ostream& PlayerNedependent::write(std::ostream& os) {
  os << "\nPlayer-ul " << name << " a ramas cu balanta: " << balance;
  return os;
}
