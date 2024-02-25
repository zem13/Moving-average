#include "moving_avg.hpp"

#include <chrono>
#include <iostream>
#include <thread>

/// @brief Generate random float [0.0; 1000.0]
static inline float getRandomFloat()
{
    return float(float(rand() % 1000000) / 1000);
}

int main()
{
    const size_t data_size = 1000000;
    std::vector<float> data_float(data_size, 0);
    std::vector<double> data_double(data_size, 0);

    for (int i = 0; i < data_size; ++i)
    {
        data_float.at(i) = getRandomFloat();
        data_double.at(i) = double(data_float.at(i));
    }

    size_t windowSize = 3;

    auto start_double_reduce = std::chrono::high_resolution_clock::now();
    std::vector<double> result_double_reduce = simpleMovingAverageMapReduce<double>(data_double, windowSize, std::thread::hardware_concurrency() - 1);
    auto end_double_reduce = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_double_reduce = end_double_reduce - start_double_reduce;
    std::cout << "Time taken for double reduce data: " << elapsed_double_reduce.count() << " seconds\n";

    return 0;
}