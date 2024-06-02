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

int main() {
    try {
        auto app = std::make_unique<Application>("Pacanea");
        Project project(std::move(app));
        project.start();
    } catch (const CasinoException& e) {
        std::cerr << "Exception caught in main(): " << e.what() << '\n';
    }
    return 0;
}

