#include <iostream>
#include <fstream>
#include <algorithm> 
#include <string>
#include <cmath>

#include "utils.h"

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
}