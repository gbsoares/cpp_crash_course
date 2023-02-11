#include <cstdio>
#include <sys/time.h>
#include <unistd.h>
#include <cstring> 

struct TimerClass
{
    /* contructor */
    TimerClass(const char* tag)
    {
        strcpy(this->tag, tag);
        gettimeofday(&timestamp, nullptr);
        printf("%s start: %ld\n", this->tag, this->timestamp.tv_sec);
    }

    /* copy constructor */
    TimerClass(const TimerClass& other) : 
        timestamp(other.timestamp)
    {
        strcpy(this->tag, other.tag);
    }

    /* copy assignment */
    TimerClass& operator=(const TimerClass& other)
    {
        if(this == &other)
            return *this;
        
        this->timestamp = other.timestamp;
        strcpy(this->tag, other.tag);
        return *this;
    }

    /* destructor */
    ~TimerClass()
    {
        struct timeval toc;
        gettimeofday(&toc, nullptr);
        printf("%s age: %ld us\n", this->tag, toc.tv_usec - this->timestamp.tv_usec);
    }

    private:
        struct timeval timestamp;
        char tag[256];
};


int main(int argc, char** argv)
{
    TimerClass tc_a("A");
    sleep(1);
    TimerClass tc_b("B");
    /* copy by construction */
    TimerClass tc_c(tc_a);
    /* copy by assignment */
    tc_c = tc_b;

    return 0;
}