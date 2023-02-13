#include <cstdio>
#include <map>
#include<type_traits>

template <typename T>
concept Integral = std::is_integral<T>::value;

template <Integral T, size_t Length>
T mode(const T (&values)[Length])
{
    if(!values)
        return 0;
    
    std::map<T, int> histogram {};
    int mode_count {};

    for(int i = 0; i < Length; i++)
    {
        histogram[values[i]]++;
        mode_count = std::max(mode_count, histogram[values[i]]);
    }

    int num_modes {};
    T mode {};
    for(auto el : histogram)
    {
        if(el.second == mode_count)
        {
            /* more than one mode... */
            if(num_modes > 0)
                return 0;
            else
            {
                num_modes++;
                mode = el.first;
            }
        }
    }
    return mode;
}

int main(int argc, char** argv)
{
    int int_arr[] = {5, 4, 3, 2, 1, 2, 9, 9, 2};
    int char_arr[] = {5, 4, 3, 1, 1, 1, 9, 9, 2};
    double double_arr[] = {5.0, 4.0, 3.0, 2.0, 1.0, 2.0, 9.0, 9.0, 2.0};

    printf("Mode = %d\n", mode(int_arr));
    printf("Mode = %d\n", mode(char_arr));
    // printf("Mode = %f\n", mode(double_arr));

    return 0;
}