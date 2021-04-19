#pragma once

#include <memory>
#include <string>
#include <stdexcept>

template<typename ...Args>
std::string format(const std::string& format, Args ...args) {
    int size_s = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
    if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1);
}