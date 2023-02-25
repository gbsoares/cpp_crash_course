#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <functional>
#include <stdexcept>
#include <cmath>

size_t calc_cols(size_t n_rows, size_t x)
{
    if(x % n_rows != 0)
        throw std::logic_error{"Can't generate rows of equal size"};
    return x / n_rows;
}

template <typename T>
struct Matrix
{
    constexpr Matrix(const size_t n_rows, std::initializer_list<T> val) :
        rows { n_rows },
        cols { calc_cols(n_rows, val.size()) },
        data(n_rows, std::vector<T>{})
    {
        auto itr = val.begin();
        for (size_t row{}; row < cols; row++)
        {
            data[row].assign(itr, itr + cols);
            itr += cols;
        }
    }

    T &get(size_t row, size_t col)
    {
        if (row >= rows || col >= cols)
            throw std::out_of_range{"Index out of range."};
        return data[row][col];
    }

    const size_t rows;
    const size_t cols;

private:
    std::vector<std::vector<T>> data;
};

TEST_CASE("Matrix and std::initializer_list")
{
    Matrix<int> mat(4, {
        1,   2,  3,  4, 
        5,   0,  7,  8, 
        9,  10, 11, 12, 
        13, 14, 15, 16
    });
    REQUIRE(mat.rows == 4);
    REQUIRE(mat.cols == 4);
    mat.get(1, 1) = 6;
    REQUIRE(mat.get(1, 1) == 6);
    REQUIRE(mat.get(0, 2) == 3);
}

TEST_CASE("Matrix with incorrect number of elements for 4 rows")
{
    try
    {
        Matrix<int> mat(4, {
            1,   2,  3,  4, 
            5,   0,  7,  8, 
            9,  10, 11, 12, 
            13, 14, 15, 16, 17
        });
    }
    catch(const std::exception& e)
    {
       REQUIRE_NOTHROW(throw e.what());
    }
}