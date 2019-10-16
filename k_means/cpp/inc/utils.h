#include <iostream>
#include <fstream>
#include <vector>

#pragma once

namespace utils {
    std::vector<std::vector<std::string>> read_csv(const std::string &filename, const char &sep);
    
    template <typename T>
    std::vector<std::vector<T>> transpose(const std::vector<std::vector<T>> &v);

    std::vector<std::vector<float>> discretize(const std::vector<std::vector<std::string>> &v);
};
    
template <typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &v);

template <typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<std::vector<T>> &v);