#pragma once

#include <vector>
#include <thread>

/// @brief Calculate data_size average for data[window_size, data_size + window_size].
/// @tparam T have to support operators /, +=, -
/// @param data Pointer to data. Must be allocated at least data_size + window_size.
/// @param result Pointer to results. Must be allocated at least data_size.
template <typename T>
void simpleMovingAverage(T const *data, size_t data_size, size_t window_size, T *result)
{
    T sum = 0;
    for (int i = 0; i < window_size; i++)
    {
        sum += data[i];
    }

    for (int i = window_size; i < data_size; i++)
    {
        result[i] = sum / window_size;
        sum += data[i] - data[i - window_size];
    }
}

/// @brief Parallel simpleMovingAverage
/// @tparam T have to support operators /, +=, - and default constructor
/// @return vector with data.size() and first window_size elements = T()
template <typename T>
std::vector<T> simpleMovingAverageMapReduce(std::vector<T> data, size_t window_size, size_t threads_num)
{
    /// Calculate data size for every thread.
    size_t addition = data.size() / threads_num;
    size_t remainder = data.size() % threads_num;
    std::vector<size_t> data_sizes(threads_num, addition);
    /// Add the remaining values to the first threads to ensure that sum(data_sizes) == data.size()
    for (int i = 0; i < remainder; i++)
    {
        data_sizes[i]++;
    }

    std::vector<std::thread> threads;
    std::vector<T> results;
    results.resize(data.size());

    auto fun = [](T const *data, size_t data_size, size_t window_size, T *result)
    {
        (simpleMovingAverage(data, data_size, window_size, result));
    };

    size_t sum = 0;
    for (int i = 0; i < threads_num - 1; i++)
    {
        threads.push_back(std::thread(fun, data.data() + sum, data_sizes[i] + window_size, window_size, results.data() + sum));
        sum += data_sizes[i];
    }

    /// Last thread must not calculate for last window_size
    threads.push_back(std::thread(fun, data.data() + sum, data_sizes[threads_num - 1], window_size, results.data() + sum));

    for (auto i = threads.begin(); i != threads.end(); i++)
    {
        (*i).join();
    }

    return results;
}