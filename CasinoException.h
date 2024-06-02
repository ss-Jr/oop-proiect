#ifndef CASINO_EXCEPTION_H
#define CASINO_EXCEPTION_H

#include <exception>

class CasinoException : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Casino exception occurred";
    }
};

#endif // CASINO_EXCEPTION_H
