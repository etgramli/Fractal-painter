//
// Created by eti on 10.11.19.
//

#ifndef FRAKTAL_KERNELEXCEPTION_H
#define FRAKTAL_KERNELEXCEPTION_H

#include <exception>

class KernelException : std::exception {
    const char* description;

public:
    explicit KernelException(const char* description) {
        this->description = description;
    }
    const char* what() const noexcept override {
        return description;
    }
};


#endif //FRAKTAL_KERNELEXCEPTION_H
