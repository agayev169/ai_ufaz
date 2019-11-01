#include <iostream>
#include <vector>

template <typename T>
class matrix {
private:
    std::vector<std::vector<T>> data_;
public:
    matrix(const std::vector<std::vector<T>> &data);
    matrix(uint32_t rows, uint32_t cols);
    matrix(const matrix &m);
    ~matrix();

    std::vector<T> &operator[](const uint32_t row);
    std::vector<T> operator[](const uint32_t row) const;
    matrix<T> operator=(const matrix &m);
    matrix<T> operator+(const matrix &m) const;
    matrix<T> operator+(const T &c) const;
    matrix<T> operator+=(const matrix &m);
    matrix<T> operator+=(const T &c);
    matrix<T> operator-(const matrix &m) const;
    matrix<T> operator-(const T &c) const;
    matrix<T> operator-=(const matrix &m);
    matrix<T> operator-=(const T &c);
    matrix<T> operator*(const matrix &m) const;
    matrix<T> operator*(const T &c) const;
    matrix<T> operator*=(const matrix &m);
    matrix<T> operator*=(const T &c);
    matrix<T> operator^(const matrix &m) const;
    matrix<T> operator^=(const matrix &m);
    bool operator==(const matrix &m);
    bool operator!=(const matrix &m);

    std::vector<uint32_t> shape() const;
    std::vector<std::vector<T>> data() const;
    static matrix<double> random(uint32_t rows, uint32_t cols, double min=0, double max=1);
    static matrix<double> zeros(uint32_t rows, uint32_t cols);
    static matrix<double> ones(uint32_t rows, uint32_t cols);
    static matrix<double> identity(uint32_t n);
    matrix<T> matmul(const matrix &m) const;
    static matrix<T> matmul(const matrix &m1, const matrix &m2);
    static matrix<T> trasnpose(const matrix<T> &m);
    matrix<T> trasnpose() const;
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const matrix<T> &m);