#include "moving_avg.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <array>
#include <fstream>
#include <cmath>

namespace
{
    /// @brief Generate random float [0.0; 1000.0]
    inline float getRandomFloat()
    {
        return float(float(rand() % 1000000) / 1000);
    }

    /// @brief Test correctly of results
    bool test_avg()
    {
        const size_t data_size = 100;
        const size_t window_size = 4;
        std::vector<double> data_double(data_size, 0);
        std::vector<double> my_results(data_size, 0);

        double first_window_avg = 0.0;
        for (int i = 0; i < data_size; ++i)
        {
            data_double.at(i) = i * 0.001;

            if (i < window_size)
            {
                my_results.at(i) = 0.0;
                first_window_avg += data_double.at(i) / window_size;
            }
            else
            {
                my_results.at(i) = (data_double.at(i - window_size) + first_window_avg);
            }
        }

        auto result_double_reduce = simpleMovingAverageMapReduce(data_double, window_size, std::thread::hardware_concurrency() - 1);

        bool res = true;

        if (my_results.size() != result_double_reduce.size())
        {
            res = false;
            std::cout << "Vectors sizes are" << my_results.size() << " and " << result_double_reduce.size() << std::endl;
        }

        for (int i = 0; i < result_double_reduce.size(); i++)
        {
            if (fabs(result_double_reduce[i] - my_results[i]) > 0.00001)
            {
                res = false;
                std::cout << "Difference in index" << i << " with values " << result_double_reduce[i] << " and " << my_results[i] << std::endl;
            }
        }
        return res;
    }

    /// @brief Write time results in file
    void write_times()
    {
        const size_t data_size = 1000000;
        std::vector<float> data_float(data_size, 0);
        std::vector<double> data_double(data_size, 0);

        for (int i = 0; i < data_size; ++i)
        {
            data_float.at(i) = getRandomFloat();
            data_double.at(i) = double(data_float.at(i));
        }

        const std::array<size_t, 6> window_size = {4, 8, 16, 32, 64, 128};

        std::ofstream time_results("time_results.txt");

        if (time_results.is_open())
        {
            time_results << "Double times" << std::endl;

            for (auto w_size : window_size)
            {
                auto start_double_reduce = std::chrono::high_resolution_clock::now();
                auto result_double_reduce = simpleMovingAverageMapReduce(data_double, w_size, std::thread::hardware_concurrency() - 1);
                auto end_double_reduce = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed_double_reduce = end_double_reduce - start_double_reduce;

                time_results << "Double " << w_size << " " << elapsed_double_reduce.count() / data_size << std::endl;
            }

            time_results << "Float times" << std::endl;

            for (auto w_size : window_size)
            {
                auto start_float_reduce = std::chrono::high_resolution_clock::now();
                auto result_float_reduce = simpleMovingAverageMapReduce(data_float, w_size, std::thread::hardware_concurrency() - 1);
                auto end_float_reduce = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed_float_reduce = end_float_reduce - start_float_reduce;

                time_results << "Float " << w_size << " " << elapsed_float_reduce.count() / data_size << std::endl;
            }

            time_results.close();
        }
        else
        {
            std::cout << "Unable to open file\n";
        }
    }
}

int main()
{
    write_times();

    test_avg();

    return 0;
}