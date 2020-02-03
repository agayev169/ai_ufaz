#pragma once

#include <iostream>
#include <fstream>
#include <algorithm> 
#include <string>
#include <cmath>

namespace utils {
    std::vector<std::vector<std::string>> read_csv(const std::string &filename, const char &sep);

    template <typename T>
    void write_csv(const std::string &filename, const std::vector<std::vector<T>> &data, char sep=',');
    
    template <typename T>
    std::vector<std::vector<T>> transpose(const std::vector<std::vector<T>> &v);
    
    template <typename T>
    std::vector<std::vector<T>> reshape(const std::vector<std::vector<T>> &v, const std::vector<size_t> &shape);
    
    template <typename T>
    std::vector<std::vector<T>> reshape(const std::vector<T> &v, const std::vector<size_t> &shape);

    std::vector<std::vector<float>> to_float(const std::vector<std::vector<std::string>> &v);

    std::vector<std::vector<double>> to_double(const std::vector<std::vector<std::string>> &v);

    template <typename T>
    std::vector<std::vector<T>> sort_by(const std::vector<std::vector<T>> &v, uint32_t row=-1, uint32_t col=-1);

    template <typename T>
    std::vector<std::vector<T>> get_cols(const std::vector<std::vector<T>> &v, const std::vector<uint32_t> &cols);

    template <typename T>
    std::vector<std::vector<T>> concat(const std::vector<std::vector<T>> &v1, const std::vector<std::vector<T>> &v2, size_t axis=0);

    template <typename T1, typename T2>
    std::vector<std::vector<T1>> concat(const std::vector<std::vector<T1>> &v1, const std::vector<std::vector<T2>> &v2, size_t axis=0);

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

std::vector<std::vector<std::string>> utils::read_csv(const std::string &filename, const char &sep=',') {
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> line;

    std::ifstream in(filename.c_str());

    char c;
    c = in.get();
    std::string cur_val;
    while (c != EOF) {
        while (c != sep && c != '\n' && c != EOF) {
            cur_val += c;
            c = in.get();
        }
        
        line.push_back(cur_val);
        cur_val = "";
        
        if (c == '\n') {
            result.push_back(line);
            line.erase(line.begin(), line.end());
        }

        c = in.get();
    }


    return result;
}


template <typename T>
void utils::write_csv(const std::string &filename, const std::vector<std::vector<T>> &data, char sep) {
    std::ofstream out(filename.c_str());

    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[i].size(); ++j) {
            out << data[i][j];
            if (j != data[i].size() - 1) out << sep;
        }
        out << std::endl;
    }
}

template <typename T>
std::vector<std::vector<T>> utils::transpose(const std::vector<std::vector<T>> &v) {
    std::vector<std::vector<T>> res;
    
    for (uint32_t i = 0; v.size() > 0 && i < v[0].size(); ++i) {
        std::vector<T> cur_col;
        for (uint32_t j = 0; j < v.size(); ++j) {
            if (v[j].size() > i) {
                cur_col.push_back(v[j][i]);
            }
        }
        res.push_back(cur_col);
    }

    return res;
}

template <typename T>
std::vector<std::vector<T>> utils::reshape(const std::vector<std::vector<T>> &v, const std::vector<size_t> &shape) {
    if (shape[0] * shape[1] != v.size() * v[0].size()) {
        throw "Cannot reshape!";
    }

    std::vector<std::vector<T>> result(shape[0], std::vector<T>(shape[1]));

    for (size_t i = 0; i < shape[0]; ++i) {
        for (size_t j = 0; j < shape[1]; ++j) {
            size_t k = (i * shape[1] + j) / v[0].size();
            size_t l = (i * shape[1] + j) % v[0].size();

            result[i][j] = v[k][l];
        }
    }
    
    return result;
}

template <typename T>
std::vector<std::vector<T>> utils::reshape(const std::vector<T> &v, const std::vector<size_t> &shape) {
    if (shape.size() != 2 || shape[0] * shape[1] != v.size()) {
        throw "Cannot reshape!";
    }

    std::vector<std::vector<T>> result(shape[0], std::vector<T>(shape[1]));

    for (size_t i = 0; i < shape[0]; ++i) {
        for (size_t j = 0; j < shape[1]; ++j) {
            size_t k = (i * shape[1] + j);

            result[i][j] = v[k];
        }
    }
    
    return result;
}

bool is_float(std::string val) {
    bool res = true;

    uint8_t start = 0; 
    if (val.size() > 0 && val[0] == '-') start = 1;

    for (uint32_t i = start; i < val.size(); ++i) {
        if (!((val[i] >= '0' && val[i] <= '9') || val[i] == '.')) res = false;
    }

    return res;
}

std::vector<std::vector<float>> utils::to_float(const std::vector<std::vector<std::string>> &v) {
    std::vector<std::vector<std::string>> tmp = utils::transpose(v);
    std::vector<std::vector<float>> res;

    for (uint32_t i = 0; i < tmp.size(); ++i) {
        bool floatable = true;
        for (uint32_t j = 0; j < v[i].size(); ++j) {
            if (!is_float(tmp[i][j])) {
                floatable = false;
                break;
            }
        }

        std::vector<float> line;
        
        if (floatable) {
            for (uint32_t j = 0; j < tmp[i].size(); ++j) {
                line.push_back(std::stof(tmp[i][j]));
            }
        } else {
            std::vector<std::string> unique;
            for (uint32_t j = 0; j < tmp[i].size(); ++j) {
                unique.push_back(tmp[i][j]);                
            }
            auto it = std::unique(unique.begin(), unique.end());
            unique.resize(std::distance(unique.begin(), it));
            
            for (uint32_t j = 0; j < tmp[i].size(); ++j) {
                for (uint32_t k = 0; k < unique.size(); ++k) {
                    if (tmp[i][j] == unique[k]) {
                        line.push_back((float) k);
                    }
                }
            }    
        }
        res.push_back(line);
    }

    return utils::transpose(res);
}

std::vector<std::vector<double>> utils::to_double(const std::vector<std::vector<std::string>> &v) {
    std::vector<std::vector<std::string>> tmp = utils::transpose(v);
    std::vector<std::vector<double>> res;

    for (uint32_t i = 0; i < tmp.size(); ++i) {
        bool floatable = true;
        for (uint32_t j = 0; j < v[i].size(); ++j) {
            if (!is_float(tmp[i][j])) {
                floatable = false;
                break;
            }
        }

        std::vector<double> line;
        
        if (floatable) {
            for (uint32_t j = 0; j < tmp[i].size(); ++j) {
                line.push_back(std::stof(tmp[i][j]));
            }
        } else {
            std::vector<std::string> unique;
            for (uint32_t j = 0; j < tmp[i].size(); ++j) {
                unique.push_back(tmp[i][j]);                
            }
            auto it = std::unique(unique.begin(), unique.end());
            unique.resize(std::distance(unique.begin(), it));
            
            for (uint32_t j = 0; j < tmp[i].size(); ++j) {
                for (uint32_t k = 0; k < unique.size(); ++k) {
                    if (tmp[i][j] == unique[k]) {
                        line.push_back((double) k);
                    }
                }
            }    
        }
        res.push_back(line);
    }

    return utils::transpose(res);
}

template <typename T> 
std::vector<std::vector<T>> utils::sort_by(const std::vector<std::vector<T>> &v, uint32_t row, uint32_t col) {
    if ((row == -1 && col == -1) || (row != -1 && col != -1)) {
        throw "Sorting should be done either by row or by column";
    }

    std::vector<std::vector<T>> res;

    // TODO: implement

    return res;
}

template <typename T>
std::vector<std::vector<T>> utils::get_cols(const std::vector<std::vector<T>> &v, const std::vector<uint32_t> &cols) {
    if (cols.size() == 0) return std::vector<std::vector<T>>();
    std::vector<std::vector<T>> res(v.size(), std::vector<T>(cols.size()));

    for (uint32_t i = 0; i < cols.size(); ++i) {
        if (cols[i] >= v.size()) {
            throw "Column out of bounds of array";
        }

        for (uint32_t j = 0; j < v.size(); ++j) {
            res[j][i] = v[j][cols[i]];
        }
    }

    return res;
}

template <typename T>
std::vector<std::vector<T>> utils::concat(const std::vector<std::vector<T>> &v1, const std::vector<std::vector<T>> &v2, size_t axis) {
    if ((axis == 0 && v1[0].size() != v2[0].size()) || (axis == 1 && v1.size() != v2.size()) || (axis != 0 && axis != 1)) {
        throw "Cannot concatenate vectors";
    }

    std::vector<std::vector<T>> result((axis == 0) ? (v1.size() + v2.size()) : (v1.size()), 
                                       std::vector<T>((axis == 0) ? (v1.size()) : (v1.size() + v2.size())));
    
    if (axis == 0) {
        size_t res_count = 0;
        for (size_t i = 0; i < v1.size(); ++i, ++res_count) {
            result[res_count] = v1[i]; 
        }

        for (size_t i = 0; i < v2.size(); ++i, ++res_count) {
            result[res_count] = v2[i];
        }
    } else {
        for (size_t i = 0; i < v1.size(); ++i) {
            size_t res_count = 0;
            for (size_t j = 0; j < v1[i].size(); ++j, ++res_count) {
                result[i][res_count] = v1[i][j];
            }

            for (size_t j = 0; j < v2[i].size(); ++j, ++res_count) {
                result[i][res_count] = v2[i][j];
            }
        }
    }

    return result;
}

template <typename T1, typename T2>
std::vector<std::vector<T1>> utils::concat(const std::vector<std::vector<T1>> &v1, const std::vector<std::vector<T2>> &v2, size_t axis) {
    if ((axis == 0 && v1[0].size() != v2[0].size()) || (axis == 1 && v1.size() != v2.size()) || (axis != 0 && axis != 1)) {
        throw "Cannot concatenate vectors";
    }

    std::vector<std::vector<T1>> result((axis == 0) ? (v1.size() + v2.size()) : (v1.size()), 
                                        std::vector<T1>((axis == 0) ? (v1[0].size()) : (v1[0].size() + v2[0].size())));
    
    if (axis == 0) {
        size_t res_count = 0;
        for (size_t i = 0; i < v1.size(); ++i, ++res_count) {
            result[res_count] = v1[i]; 
        }

        for (size_t i = 0; i < v2.size(); ++i, ++res_count) {
            for (size_t j = 0; j < v2[i].size(); ++j) {
                result[res_count][j] = (T1) v2[i][j];
            }
        }
    } else {
        for (size_t i = 0; i < v1.size(); ++i) {
            size_t res_count = 0;
            for (size_t j = 0; j < v1[i].size(); ++j, ++res_count) {
                result[i][res_count] = v1[i][j];
            }

            for (size_t j = 0; j < v2[i].size(); ++j, ++res_count) {
                result[i][res_count] = (T1) v2[i][j];
            }
        }
    }

    return result;
}

template <typename T>
double utils::dist(const std::vector<T> &a, const std::vector<T> &b) {
    if (a.size() != b.size()) {
        throw utils::str_exception("Size of the elements must be the same to compute the distance");
    }

    T res = 0;

    for (std::size_t i = 0; i < a.size(); ++i) {
        res += (a[i] - b[i]) * (a[i] - b[i]);
    }

    return sqrt(res);
}

template <typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &v) {
    stream << "[";
    for (uint32_t i = 0; i < v.size(); ++i) {
        stream << v[i];
        if (i < v.size() - 1) stream << ", ";
    }
    stream << "]";
    return stream;
}

template <typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<std::vector<T>> &v) {
    stream << "[";
    for (uint32_t i = 0; i < v.size(); ++i) {
        if (i != 0) stream << " ";
        stream << v[i];
        if (i < v.size() - 1) stream << "," << std::endl;
    }
    stream << "]";
    return stream;
}