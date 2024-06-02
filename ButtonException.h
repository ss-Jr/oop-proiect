#ifndef BUTTON_EXCEPTION_H
#define BUTTON_EXCEPTION_H

#include "CasinoException.h"

class ButtonException : public CasinoException {
public:
    virtual const char* what() const noexcept override {
        return "Button exception occurred";
    }
};

#endif // BUTTON_EXCEPTION_H

