#pragma once
#include <stdexcept>
#include <string>

class GraphicError : std::runtime_error {
public:
    explicit GraphicError(const std::string& what_arg) : std::runtime_error(what_arg) {
    }
    explicit GraphicError(const char* what_arg) : std::runtime_error(what_arg) {
    }
};
