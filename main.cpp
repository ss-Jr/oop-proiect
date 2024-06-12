#include <array>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <exception>

#include "Application.h"
#include "Project.h"
#include "Location.h"
#include "GameException.h"
#include "ButtonException.h"
#include "Button.h"
#include "Buttons.h"
#include "Location.h"
#include "Player.h"
#include "PlayerDependent.h"
#include "PlayerNedependent.h"

int main() {
    try {
        auto app = std::make_unique<Application>("Pacanea");
        auto cloned_app = app->clone();
        delete cloned_app;
        Project project(std::move(app));
        std::string tip;
        int n;
        std::cout << "Introdu nr-ul de player: ";
        std::cin >> n;
        std::cin.get();
        for(int i = 0; i < n; ++i) {
          Player* p = nullptr;
          std::cout << "\nAlege tip-ul de player [dependent, nedependent]: ";
          getline(std::cin, tip);
          if(tip == "dependent") {
            p = new PlayerDependent();
          } else if(tip == "nedependent") {
            p = new PlayerNedependent(); 
          } else {
            std::cout << "Ai ales un tip gresit!\n";
            continue;
          }
          std::cin >> *p;
          project.setPlayer(p);
        }
        project.start();
        std::cout << project;
    } catch (const CasinoException& e) {
        std::cerr << "Exception caught in main(): " << e.what() << '\n';
    } 
    return 0;
}

