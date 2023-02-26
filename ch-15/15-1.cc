#include <cstdint>
#include <cstdio>
#include <string>
#include <array>

struct AlphaHistogram
{
    void ingest(std::string_view str);
    void print() const;

private:
    std::array<size_t, 26> counts{};
};

void AlphaHistogram::ingest(std::string_view str)
{
    for(auto c : str)
    {
        if(c >= 'A' && c <= 'Z') counts[c - 'A']++;
        else if(c >= 'a' && c <= 'z') counts[c - 'a']++;
    }
}

void AlphaHistogram::print() const
{
    for (auto index{'A'}; index <= 'Z'; index++)
    {
        printf("%c: ", index);
        auto n_asterisks = counts[index - 'A'];
        while (n_asterisks--)
            printf("*");
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    AlphaHistogram hist;
    for (size_t i{1}; i < argc; i++)
    {
        hist.ingest(std::string(argv[i]));
    }
    hist.print();
}