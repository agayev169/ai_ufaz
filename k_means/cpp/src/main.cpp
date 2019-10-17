#include <iostream>
#include <cstdlib>
#include "../inc/utils.cpp"
#include "../inc/k_means.cpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] <<" file" << std::endl;
        exit(1); 
    }

    // std::vector<std::vector<std::string>> data = utils::read_csv(argv[1]);

    // for (std::vector<std::string> line : data) {
    //     std::cout << line << std::endl;
    // }

    // std::cout << data << std::endl;
    // std::cout << utils::transpose(data) << std::endl;

    std::vector<std::vector<float>> data = utils::to_float(utils::read_csv(argv[1]));
    // std::cout << data << std::endl;
    // std::cout << utils::get_cols(data, {0, 1, 2, 3}) << std::endl;

    k_means<float> k(utils::get_cols(data, {0, 1, 2, 3}), 3, 1000, 1e-5, k_means<float>::K_MEANS_PP);
    std::vector<std::vector<float>> centroids = k.get_centroids();
    std::cout << centroids << std::endl;

    return 0;
}