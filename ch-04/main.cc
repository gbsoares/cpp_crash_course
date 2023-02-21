#include <cstdio>
#include <sys/time.h>
#include <unistd.h>
#include <cstring> 
#include <utility>

struct TimerClass
{
    /* contructor */
    TimerClass(const char* name)
    {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);

        gettimeofday(&timestamp, nullptr);
        printf("%s start\n", this->name);
    }

    /* copy constructor */
    TimerClass(const TimerClass& other) : 
        timestamp{ other.timestamp },
        name{ new char[strlen(other.name) + 1]}
    {
        strcpy(this->name, other.name);

        printf("%s copied by construction\n", other.name);
    }

    /* copy assignment */
    TimerClass& operator=(const TimerClass& other)
    {
        if(this == &other)
            return *this;

        printf("%s copied to %s by assignment\n", other.name, this->name);

        this->timestamp = other.timestamp;
        /* create new buffer, delete the old, and copy name */
        const auto new_name = new char[strlen(other.name) + 1];
        delete[] this->name;
        this->name = new_name;
        strcpy(this->name, other.name);

        return *this;
    }

    /* move constructor */
    TimerClass(TimerClass&& other) noexcept :
        timestamp{ other.timestamp },
        name{ other.name }
    {
        other.name = nullptr;
        other.timestamp.tv_sec = 0;
        other.timestamp.tv_usec = 0;

        printf("%s moved by construction\n", this->name);
    }

    /* move assignment */
    TimerClass& operator=(TimerClass&& other) noexcept
    {
        if(this == &other)
            return *this;

        printf("%s moved to %s by assignment\n", other.name, this->name);
        
        this->timestamp = other.timestamp;
        /* delete old buffer, and grab the new one */
        delete[] this->name;
        this->name = other.name;

        other.name = nullptr;
        other.timestamp.tv_sec = 0;
        other.timestamp.tv_usec = 0;

        return *this;
    }

    /* destructor */
    ~TimerClass()
    {
        if(this->timestamp.tv_usec > 0)
        {
            struct timeval toc;
            gettimeofday(&toc, nullptr);
            printf("%s age: %ld us\n", this->name, toc.tv_usec - this->timestamp.tv_usec);
        }

        if(this->name != nullptr)
            delete[] this->name;
    }

    private:
        struct timeval timestamp;
        char* name;
};


int main(int argc, char** argv)
{
    printf("Create tc_a: ");
    TimerClass tc_a("A");

    printf("Create tc_b: ");
    TimerClass tc_b("B");

    /* copy by construction */
    printf("Create tc_c: ");
    TimerClass tc_c(tc_a);
    
    /* copy by assignment */
    printf("Copy tc_b to tc_c: ");
    tc_c = tc_b;

    /* move by construction: tc_d now owns tc_a */
    printf("Create tc_d, taking ownership of elements from tc_a: ");
    TimerClass tc_d(std::move(tc_a));

    /* move by assignment: tc_a takes ownership of tc_b */
    printf("Move tc_b to tc_a: ");
    tc_a = std::move(tc_b);

    return 0;
}