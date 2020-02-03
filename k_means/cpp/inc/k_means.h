#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>

#include "../inc/utils.h"

template <typename T>
class k_means {
private:
    std::vector<std::vector<T>>         centroids_;
    uint32_t                            clusters_n_;
    
    void update_centroids(const std::vector<std::vector<T>> &data, const std::vector<std::size_t> &clusters); 
public:
    const static uint8_t RANDOM     = 0;
    const static uint8_t K_MEANS_PP = 1;

    k_means(const std::vector<std::vector<T>> &data, 
            uint32_t clusters_n, 
            uint32_t iters=1000, 
            float threshold=0.00001, 
            uint8_t initializer=RANDOM, 
            bool verbose=true);
    ~k_means();

    std::vector<std::vector<T>> get_centroids() const;

    std::vector<size_t> predict(const std::vector<std::vector<T>> &data) const;
};

template <typename T>
k_means<T>::k_means(const std::vector<std::vector<T>> &data, uint32_t clusters_n, uint32_t iters, 
                    float threshold, uint8_t initializer, bool verbose): 
                    clusters_n_(clusters_n) {
    srand(time(NULL));

    if (initializer == k_means::RANDOM) {
        for (uint32_t i = 0; i < clusters_n; ++i) {
            centroids_.push_back(data[rand() % data.size()]);
        }
    } else if (initializer == k_means::K_MEANS_PP) {
        centroids_.push_back(data[rand() % data.size()]);

        for (uint32_t i = 0; i < clusters_n - 1; ++i) {
            // Compute distances between all the data points with the closest centroid
            std::vector<double> distances(data.size());
            for (size_t j = 0; j < data.size(); ++j) {
                double dist_min = std::numeric_limits<double>::max();
                for (size_t k = 0; k < centroids_.size(); ++k) {
                    double cur_dist = utils::dist(data[j], centroids_[k]);
                    if (cur_dist < dist_min) {
                        dist_min = cur_dist;
                    }
                }
                distances[j] = dist_min;
            }

            // Compute d^2
            for (double &d : distances) {
                d = d * d;
            }

            // Choosing a new centroid
            size_t argmax = std::distance(distances.begin(), std::max_element(distances.begin(), distances.end()));
            centroids_.push_back(data[argmax]);
        }
    } else {
        throw utils::str_exception("Undefined initializer for k_means class");
    }

    std::vector<size_t> clusters(data.size());

    for (uint32_t i = 0; i < iters; ++i) {
        clusters = predict(data);
        update_centroids(data, clusters);
    }
}

template <typename T>
k_means<T>::~k_means() {;}

template <typename T>
void k_means<T>::update_centroids(const std::vector<std::vector<T>> &data, const std::vector<size_t> &clusters) {
    int static func_call_count = 0;
    ++func_call_count;
    std::vector<std::vector<T>> new_centroids(clusters_n_, std::vector<T>(data[0].size(), 0));
    std::vector<size_t> clusters_count(clusters_n_);

    for (size_t i = 0; i < clusters.size(); ++i) {
        for (size_t j = 0; j < data[i].size(); ++j) {
            new_centroids[clusters[i]][j] += data[i][j];
        }
        ++clusters_count[clusters[i]];
    }

    for (size_t i = 0; i < clusters_n_; ++i) {
        for (size_t j = 0; j < new_centroids[i].size(); ++j) {
            new_centroids[i][j] /= clusters_count[i];
        }
    }

    for (size_t i = 0; i < clusters_n_; ++i) {
        for (size_t j = 0; j < new_centroids[i].size(); ++j)
            centroids_[i][j] = new_centroids[i][j];
    }
}


template <typename T>
std::vector<size_t> k_means<T>::predict(const std::vector<std::vector<T>> &data) const {
    std::vector<size_t> clusters(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        double dist_min = std::numeric_limits<double>::max();
        size_t index = 0;

        for (size_t j = 0; j < centroids_.size(); ++j) {
            double cur_dist = utils::dist(data[i], centroids_[j]);
            if (cur_dist < dist_min) {
                dist_min = cur_dist;
                index = j;
            }
        }
        clusters[i] = index;
    }

    return clusters;
}


template <typename T>
std::vector<std::vector<T>> k_means<T>::get_centroids() const {
    return centroids_;
}