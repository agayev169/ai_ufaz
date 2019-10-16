#include <iostream>
#include <fstream>
#include <algorithm> 
#include <string>
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

std::vector<std::vector<float>> utils::discretize(const std::vector<std::vector<std::string>> &v) {
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