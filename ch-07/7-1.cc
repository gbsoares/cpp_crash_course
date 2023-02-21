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


    UnsignedBigInteger operator+=(const unsigned long long other) const
    {
        printf("Overloaded + operator w/ long long param\n");
        /* convert long long to UnsignedBigInteger */
        UnsignedBigInteger other_ubi{other};

        /* call the + operator that takes an UnsignedBigInteger */
        return *this + other_ubi;
    }

    UnsignedBigInteger operator+(const UnsignedBigInteger& other) const
    {
        /* TODO: need to check overflow */

        printf("Overloaded + operator w/ UnsignedBigInteger param\n");
        uint8_t result[1024]{};
        size_t result_size{};

        size_t i{}, j{};
        int carry{};
        while(result_size < sizeof(result) && i < rep_length && j < other.rep_length)
        {
            auto val = rep[i++] + other.rep[j++] + carry;
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
        while(j < other.rep_length)
        {
            result[result_size++] = other.rep[j++];
        }
        
        return UnsignedBigInteger{result, result_size};
    }

    UnsignedBigInteger operator-=(const unsigned long long other) const
    {
        printf("Overloaded - operator w/ long long param\n");
        UnsignedBigInteger other_ubi{other};
        return *this - other_ubi;
    }


    UnsignedBigInteger operator-(const UnsignedBigInteger& other) const
    {
        /* TODO: need to check underflow */

        printf("Overloaded - operator w/ UnsignedBigInteger param\n");
        uint8_t result[1024]{};
        size_t result_size{};

        size_t i{}, j{};
        int carry{};
        while(i < rep_length && j < other.rep_length)
        {
            auto val = rep[i++] - other.rep[j++] - carry;
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

    operator int() const
    {
        printf("Overloaded int operator\n");
        int ret{};
        int mult = 1;
        for(size_t i{}; i < rep_length && ret >= 0; i++)
        {
            ret += rep[i] * mult;
            mult *= 10;
        }
        /* narrowing happens if int value overflows and produces a negative number */
        if(ret < 0)
            throw std::runtime_error{"Narrowing"};

        return ret;
    }

    uint8_t rep[1024];
    size_t rep_length;
};

int main(int argc, char** argv)
{
    const UnsignedBigInteger a{std::numeric_limits<uint64_t>::max()};
    printf("-------\n");
    
    UnsignedBigInteger x1 = a + UnsignedBigInteger{98765498761321654};
    printf("-------\n");
    
    UnsignedBigInteger x2{99999999};
    x2 += 4567893;
    printf("-------\n");

    UnsignedBigInteger y1 = a - UnsignedBigInteger{98765498761321654};
    printf("-------\n");
    
    UnsignedBigInteger y2{99999999};
    y2 -= 4567893;
    printf("-------\n");

    /* cast large number to int (narrowing) */
    try
    {
        printf("int(a) = %d\n", (int)a);
    }
    catch(std::runtime_error e)
    {
        printf("Exception! %s\n", e.what());
    }
    printf("-------\n");
    
    /* cast number to int (not narrowing) */
    try
    {
        printf("int(p) = %d\n", (int)UnsignedBigInteger{123456});
    }
    catch(std::runtime_error e)
    {
        printf("Exception! %s\n", e.what());
    }
    printf("-------\n");

    return 0;
}