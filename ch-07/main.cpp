#include <cstdio>
#include <cstdint>
#include <limits>
#include <cstring>
#include <stdexcept>

struct UnsignedBigInteger
{
    UnsignedBigInteger(unsigned long long u_big_int)
    {
        size_t num_digits{};
        auto x = u_big_int;
        while(x)
        {
            x /= 10;
            num_digits++;
        }
        printf("%llu has %ld digits\n", u_big_int, num_digits);

        rep_length = num_digits;

        for(size_t i{}; i < num_digits; i++, u_big_int /= 10)
        {
            auto digit = u_big_int % 10;
            rep[i] = digit;
        }
    }

    UnsignedBigInteger(uint8_t *long_num, size_t size)
    {
        if(size <= sizeof(rep))
        {
            memcpy(rep, long_num, size);
            rep_length = size;
            printf("value = ");
            for(int i{} ; i < size; i++)
            {
                printf("%d", rep[size-1-i]);
            }
            printf("\n");
        }
        else
            throw std::runtime_error{"Overflow"};
    }

    UnsignedBigInteger operator+(unsigned long long other)
    {
        uint8_t result[1024]{};
        size_t result_size{};

        /* do digit-wise addition */
        UnsignedBigInteger other_ubi{other};

        size_t i{}, j{};
        int carry{};
        while(result_size < sizeof(result) && i < rep_length && j < other_ubi.rep_length)
        {
            auto val = rep[i++] + other_ubi.rep[j++] + carry;
            if(val > 10)
            {
                val -= 10;
                carry = 1;
            }
            else
                carry = 0;
            result[result_size++] = val;
        }
        while(i < rep_length)
        {
            result[result_size++] = rep[i++];
        }
        while(j < other_ubi.rep_length)
        {
            result[result_size++] = other_ubi.rep[j++];
        }
        
        return UnsignedBigInteger{result, result_size};
    }

    UnsignedBigInteger operator-(unsigned long long other)
    {
        uint8_t result[1024]{};
        size_t result_size{};

        /* do digit-wise subtraction */
        UnsignedBigInteger other_ubi{other};

        size_t i{}, j{};
        int carry{};
        while(i < rep_length && j < other_ubi.rep_length)
        {
            auto val = rep[i++] - other_ubi.rep[j++] - carry;
            if(val < 0)
            {
                val += 10;
                carry = 1;
            }
            else
            {
                carry = 0;
            }
            result[result_size++] = val;
        }
        while(i < rep_length)
        {
            auto val = rep[i++] - carry;
            if(val < 0)
            {
                val += 10;
                carry = 1;
            }
            else
            {
                carry = 0;
            }
            result[result_size++] = val;
        }

        /* check if we can trim leading zeros */
        size_t y = result_size - 1;
        while(y > 0 && result[y--] == 0) result_size--;
        
        return UnsignedBigInteger{result, result_size};
    }

    uint8_t rep[1024];
    size_t rep_length;
};

int main(int argc, char** argv)
{
    UnsignedBigInteger a{std::numeric_limits<uint64_t>::max()};
    UnsignedBigInteger x = a + (long)4566345343;
    UnsignedBigInteger y = a - (long)4566345343;

    return 0;
}