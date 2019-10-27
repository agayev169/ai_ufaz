#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <assert.h>

#include "matrix.h"

template <typename T>
matrix<T>::matrix(const std::vector<std::vector<T>> &data) : data_(data) {}

template <typename T>
matrix<T>::matrix(uint32_t rows, uint32_t cols) : data_(std::vector<std::vector<T>>(rows, std::vector<T>(cols))) {}

template <typename T>
matrix<T>::matrix(const matrix<T> &m) : data_(m.data()) {}

template <typename T>
std::vector<uint32_t> matrix<T>::shape() const {
    uint32_t sh[] = {(uint32_t) data_.size(), (uint32_t) data_[0].size()};
    return std::vector<uint32_t>(sh, sh + 2);
}

template <typename T>
std::vector<std::vector<T>> matrix<T>::data() const {
    return data_;
}


template <typename T>
matrix<T>::~matrix() {}

template <typename T>
std::vector<T> &matrix<T>::operator[](const uint32_t row) {
    return data_[row];
}

template <typename T>
std::vector<T> matrix<T>::operator[](const uint32_t row) const {
    return data_[row];
}

template <typename T>
matrix<T> matrix<T>::operator=(const matrix<T> &m) {
    data_ = m;
    return *this;
}

template <typename T>
matrix<T> matrix<T>::operator+(const matrix &m) const {
    auto sh1 = m.shape();
    auto sh2 = shape();
    assert(sh1[0] == sh2[0] && sh1[1] == sh2[1]);

    matrix<T> res(sh1[0], sh1[1]);
    for (uint32_t i = 0; i < sh1[0]; ++i) {
        for (uint32_t j = 0; j < sh1[1]; ++j) {
            res = m[i][j] + data_[i][j];
        }
    }

    return res;
}

template <typename T>
matrix<T> matrix<T>::operator+=(const matrix<T> &m) {
    auto sh1 = m.shape();
    auto sh2 = shape();
    assert(sh1[0] == sh2[0] && sh1[1] == sh2[1]);

    for (uint32_t i = 0; i < sh1[0]; ++i) {
        for (uint32_t j = 0; j < sh1[1]; ++j) {
            data_[i][j] += m[i][j];
        }
    }

    return *this;
}

template <typename T>
matrix<T> matrix<T>::operator-(const matrix<T> &m) const {
    auto sh1 = m.shape();
    auto sh2 = shape();
    assert(sh1[0] == sh2[0] && sh1[1] == sh2[1]);

    matrix<T> res(sh1[0], sh1[1]);
    for (uint32_t i = 0; i < sh1[0]; ++i) {
        for (uint32_t j = 0; j < sh1[1]; ++j) {
            res[i][j] = data_[i][j] - m[i][j];
        }
    }

    return res;
}

template <typename T>
matrix<T> matrix<T>::operator-=(const matrix<T> &m) {
    auto sh1 = m.shape();
    auto sh2 = shape();
    assert(sh1[0] == sh2[0] && sh1[1] == sh2[1]);

    for (uint32_t i = 0; i < sh1[0]; ++i) {
        for (uint32_t j = 0; j < sh1[1]; ++j) {
            data_[i][j] -= m[i][j];
        }
    }

    return *this;
}

template <typename T>
matrix<T> matrix<T>::operator*(const matrix<T> &m) const {
    auto sh1 = m.shape();
    auto sh2 = shape();
    assert(sh1[0] == sh2[0] && sh1[1] == sh2[1]);

    matrix<T> res(sh1[0], sh1[1]);
    for (uint32_t i = 0; i < sh1[0]; ++i) {
        for (uint32_t j = 0; j < sh1[1]; ++j) {
            res[i][j] = data_[i][j] * m[i][j];
        }
    }

    return res;
}

template <typename T>
matrix<T> matrix<T>::operator*=(const matrix<T> &m) {
    auto sh1 = m.shape();
    auto sh2 = shape();
    assert(sh1[0] == sh2[0] && sh1[1] == sh2[1]);

    for (uint32_t i = 0; i < sh1[0]; ++i) {
        for (uint32_t j = 0; j < sh1[1]; ++j) {
            data_[i][j] *= m[i][j];
        }
    }

    return *this;
}

template <typename T>
matrix<T> matrix<T>::operator^(const matrix<T> &m) const {
    auto sh1 = shape();
    auto sh2 = m.shape();
    assert(sh1[1] == sh2[0]);

    matrix<T> res(sh1[0], sh2[1]);
    for (uint32_t i = 0; i < sh1[0]; ++i) {
        for (uint32_t j = 0; j < sh2[1]; ++j) {
            for (uint32_t k = 0; k < sh1[1]; ++k) {
                res[i][j] += data_[i][k] * m[k][j];
            }
        }
    }

    return res;
}

template <typename T>
matrix<T> matrix<T>::operator^=(const matrix<T> &m) {
    auto sh1 = shape();
    auto sh2 = m.shape();
    assert(sh1[1] == sh2[0]);

    matrix<T> res(sh1[0], sh2[1]);
    for (uint32_t i = 0; i < sh1[0]; ++i) {
        for (uint32_t j = 0; j < sh2[1]; ++j) {
            for (uint32_t k = 0; k < sh1[1]; ++k) {
                res[i][j] += data_[i][k] * m[k][j];
            }
        }
    }

    *this = res;

    return *this;
}

template <typename T>
bool matrix<T>::operator==(const matrix<T> &m) {
    auto sh1 = shape();
    auto sh2 = m.shape();
    if (sh1[0] != sh2[0] || sh1[1] != sh2[1]) return false;
    
    for (uint32_t i = 0; i < sh1[0]; ++i) {
        for (uint32_t j = 0; j < sh1[1]; ++j) {
            if (data_[i][j] != m[i][j]) return false;
        }
    }

    return true;
}

template <typename T>
bool matrix<T>::operator!=(const matrix<T> &m) {
    return !(*this == m);
}

template <typename T>
matrix<double> matrix<T>::random(uint32_t rows, uint32_t cols, double min, double max) {
    static bool rand_init = false;
    if (!srand) {
        srand(time(NULL));
        rand_init = true;
    }

    if (max < min) {
        double tmp = min;
        min = max;
        max = tmp;
    }

    matrix<double> res(rows, cols);
    for (uint32_t i = 0; i < rows; ++i) {
        for (uint32_t j = 0; j < cols; ++j) {
            double nom   = rand() % 1000000000;
            double denom = 1000000000;
            double val = nom / denom * (max - min) + min;
            res[i][j] = val;
        }
    }
    return res;
}

template <typename T>
matrix<double> matrix<T>::zeros(uint32_t rows, uint32_t cols) {
    return matrix(rows, cols);
}

template <typename T>
matrix<double> matrix<T>::ones(uint32_t rows, uint32_t cols) {
    matrix<double> res(rows, cols);

    for (uint32_t i = 0; i < rows; ++i) {
        for (uint32_t j = 0; j < cols; ++j) {
            res[i][j] = 1;
        }
    }

    return res;
}

template <typename T>
matrix<double> matrix<T>::identity(uint32_t n) {
    matrix<double> res(n, n);

    for (uint32_t i = 0; i < n; ++i) {
            res[i][i] = 1;
    }

    return res;
}

template <typename T>
matrix<T> matrix<T>::matmul(const matrix<T> &m) const {
    return *this ^ m;    
}

template <typename T>
matrix<T> matrix<T>::matmul(const matrix<T> &m1, const matrix<T> &m2) {
    return m1 ^ m2;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const matrix<T> &m) {
    auto shape = m.shape();
    os << "[";
    for (uint32_t i = 0; i < shape[0]; ++i) {
        if (i != 0) os << " ";
        os << "[";
        for (uint32_t j = 0; j < shape[1]; ++j) {
            os << m[i][j];
            if (j != shape[1] - 1) os << " ";
        }
        os << "]";
        if (i != shape[0] - 1) os << std::endl;
    }
    os << "]";
    return os;
}