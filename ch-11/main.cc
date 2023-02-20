#include <cstdio>
#include <memory>
#include <errno.h>

using FileGuard = std::shared_ptr<FILE>;

struct Hal 
{
    Hal(FileGuard file) : file{ file } {}
    ~Hal() { fprintf(file.get(), "Stop, Dave.\n"); }
    void write_status() { fprintf(file.get(), "I'm completely operational.\n"); }
    FileGuard file;
};

void say_hello(FileGuard file)
{
    fprintf(file.get(), "HELLO DAVE\n");
}

int main()
{
    auto file = fopen("HAL9000", "w");
    if(!file) return errno;

    /* creating a shared_ptr pointing to file, 
     * with a custom deleter (wrapper around fclose) */
    FileGuard file_guard{ file, [](FILE* file){
            fprintf(file, "Closing the file\n");
            fclose(file);
        }
    };

    /* create multiple Hal objects */
    Hal h1 {file_guard};
    Hal h2 {file_guard};
    Hal h3 {file_guard};

    say_hello(file_guard);
    
    h1.write_status();
    h2.write_status();
    h3.write_status();

    return 0;
}