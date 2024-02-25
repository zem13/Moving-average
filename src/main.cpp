#include "moving_avg.hpp"

#include <chrono>
#include <iostream>

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

    auto start_float = std::chrono::high_resolution_clock::now();
    std::vector<float> result_float = simpleMovingAverage(data_float, windowSize);
    auto end_float = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_float = end_float - start_float;
    std::cout << "Time taken for float data: " << elapsed_float.count() << " seconds\n";

    auto start_double = std::chrono::high_resolution_clock::now();
    std::vector<double> result_double = simpleMovingAverage(data_double, windowSize);
    auto end_double = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_double = end_double - start_double;
    std::cout << "Time taken for double data: " << elapsed_double.count() << " seconds\n";

    return 0;
}