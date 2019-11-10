//
// Created by eti on 10.11.19.
//

#ifndef FRAKTAL_CONSTRUCTOREXCEPTION_H
#define FRAKTAL_CONSTRUCTOREXCEPTION_H

#include <exception>

class ConstructorException : public std::exception {
    const char* description;

public:
    explicit ConstructorException(const char* description) {
        this->description = description;
    }
    const char* what() const noexcept override {
        return description;
    }
};


#endif //FRAKTAL_CONSTRUCTOREXCEPTION_H
