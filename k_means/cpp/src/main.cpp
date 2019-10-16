#include <iostream>
#include <cstdlib>
#include "../inc/utils.cpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] <<" file" << std::endl;
        exit(1); 
    }

    std::vector<std::vector<std::string>> data = utils::read_csv(argv[1]);

    // for (std::vector<std::string> line : data) {
    //     std::cout << line << std::endl;
    // }

    std::cout << data << std::endl;
    std::cout << utils::transpose(data) << std::endl;

    std::cout << utils::discretize(data) << std::endl;

    return 0;
}