#ifndef MACHINE_EXCEPTION_H
#define MACHINE_EXCEPTION_H

#include "CasinoException.h"

class MachineException : public CasinoException {
public:
    virtual const char* what() const noexcept override {
        return "Machine exception occurred";
    }
};

#endif // MACHINE_EXCEPTION_H
