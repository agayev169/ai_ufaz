#include <iostream>
#include <fstream>
#include <vector>

#pragma once

namespace utils {
    std::vector<std::vector<std::string>> read_csv(const std::string &filename, const char &sep);
    
    template <typename T>
    std::vector<std::vector<T>> transpose(const std::vector<std::vector<T>> &v);

    std::vector<std::vector<float>> to_float(const std::vector<std::vector<std::string>> &v);

    std::vector<std::vector<double>> to_double(const std::vector<std::vector<std::string>> &v);

    template <typename T>
    std::vector<std::vector<T>> sort_by(const std::vector<std::vector<T>> &v, uint32_t row=-1, uint32_t col=-1);

    template <typename T>
    std::vector<std::vector<T>> get_cols(const std::vector<std::vector<T>> &v, const std::vector<uint32_t> &cols);

    struct str_exception : public std::exception {
        std::string message;

        str_exception(std::string str) : message(str) {;}
        ~str_exception() throw() {;}
        const char *what() const throw() {return message.c_str();}
    };

    template <typename T>
    double dist(const std::vector<T> &a, const std::vector<T> &b);
};
    
template <typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &v);

template <typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<std::vector<T>> &v);