#ifndef GAME_EXCEPTION_H
#define GAME_EXCEPTION_H

#include "CasinoException.h"

class GameException : public CasinoException {
public:
    virtual const char* what() const noexcept override {
        return "Game exception occurred";
    }
};

#endif // GAME_EXCEPTION_H

