#include <iostream>
#include <cstdlib>
#include <vector>
#include "../inc/utils.h"
#include "../inc/k_means.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] <<" dataset file" << std::endl;
        exit(1); 
    }

    // std::vector<std::vector<double>> data = utils::to_double(utils::read_csv(argv[1]));
    // k_means<double> k(utils::get_cols(data, {0, 1, 2, 3}), 3, 1000, 1e-5, k_means<double>::K_MEANS_PP);

    std::vector<std::vector<double>> data(10000, std::vector<double>(2));
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[i].size(); ++j) {
            double nom   = rand() % 100000;
            double denom = 100000;

            data[i][j] = nom / denom;
        }
    }

    k_means<double> k(data, 3, 1000, 1e-5, k_means<double>::K_MEANS_PP);
    std::vector<std::vector<double>> centroids = k.get_centroids();
    std::cout << centroids << std::endl;

    std::vector<std::vector<size_t>> clusters = utils::reshape(k.predict(data), std::vector<size_t>{data.size(), 1});

    std::vector<std::vector<double>> new_data = utils::concat(data, clusters, 1);
    utils::write_csv("output.csv", new_data);

    return 0;
}