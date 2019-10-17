#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>

#include "k_means.h"
#include "utils.h"

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
            for (std::size_t j = 0; j < data.size(); ++j) {
                double dist_min = std::numeric_limits<double>::max();
                for (std::size_t k = 0; k < centroids_.size(); ++k) {
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
            std::size_t argmax = std::distance(distances.begin(), std::max_element(distances.begin(), distances.end()));
            centroids_.push_back(data[argmax]);
        }
    } else {
        throw utils::str_exception("Undefined initializer for k_means class");
    }

    std::vector<std::size_t> clusters(data.size());
    for (uint32_t i = 0; i < iters; ++i) {
        for (std::size_t j = 0; j < data.size(); ++j) {
            double dist_min   = std::numeric_limits<double>::max();
            std::size_t index = 0;
            for (std::size_t k = 0; k < centroids_.size(); ++k) {
                double cur_dist = utils::dist(data[j], centroids_[k]);
                if (cur_dist < dist_min) {
                    dist_min = cur_dist;
                    index = k;
                }
            }
            clusters[j] = index;
        }

        update_centroids(data, clusters);
    }
}

template <typename T>
k_means<T>::~k_means() {;}

template <typename T>
void k_means<T>::update_centroids(const std::vector<std::vector<T>> &data, const std::vector<std::size_t> &clusters) {
    // TODO: implement!
}

template <typename T>
std::vector<std::vector<T>> k_means<T>::get_centroids() {
    return centroids_;
}