#pragma once

#include <vector>

template <typename T>
std::vector<T> simpleMovingAverage(const std::vector<T> &data, size_t windowSize)
{
    std::vector<T> result(data.size());

    T sum = 0;
    for (int i = 0; i < windowSize; i++)
    {
        sum += data.at(i);
    }

    for (int i = windowSize; i < data.size(); i++)
    {
        result.at(i) = sum / windowSize;
        sum += data.at(i) - data.at(i - windowSize);
    }

    return result;
}