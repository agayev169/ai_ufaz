#pragma once

#include <vector>

template <typename T>
class k_means {
private:
    std::vector<std::vector<T>>         centroids_;
    uint32_t                            clusters_n_;
    
    void update_centroids(const std::vector<std::vector<T>> &data, const std::vector<std::size_t> &clusters); 
public:
    const static uint8_t RANDOM     = 0;
    const static uint8_t K_MEANS_PP = 1;

    k_means(const std::vector<std::vector<T>> &data, uint32_t clusters_n, uint32_t iters=1000, float threshold=0.00001, uint8_t initializer=RANDOM, bool verbose=true);
    ~k_means();

    std::vector<std::vector<T>> get_centroids();
};