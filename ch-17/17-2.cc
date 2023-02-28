#include <filesystem>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace std::filesystem;

struct Attributes
{
    size_t size_bytes;
    size_t n_directories;
    size_t n_files;
};

void print_line(const Attributes &attributes, string_view path)
{
    cout << setw(12) << attributes.size_bytes << setw(6) << attributes.n_files << setw(6) << attributes.n_directories
         << " " << path << "\n";
}

Attributes explore(const directory_entry &directory)
{
    Attributes attributes{};
    for (const auto &entry : recursive_directory_iterator{directory.path()})
    {
        if (entry.is_directory())
        {
            attributes.n_directories++;
        }
        else
        {
            attributes.n_files++;
            attributes.size_bytes += entry.file_size();
        }
    }    
    return attributes;
}

int main(int argc, const char **argv)
{
    bool recursive = false;
    path sys_path{};

    if (argc != 2 && argc != 3)
    {
        cerr << "Usage: treedir {OPT} PATH" << endl;
        return -1;
    }
    else if(argc == 3)
    {
        std::string opt{argv[1]};
        if(opt[0] == '-' && opt[1] == 'R')
        {
            recursive = true;
        }
        sys_path = argv[2];
    }
    else
    {
        sys_path = argv[1];
    }

    
    cout << "Size         Files  Dirs Name\n";
    cout << "------------ ----- ----- ------------\n";
    Attributes root_attributes{};
    for (const auto &entry : directory_iterator{sys_path})
    {
        try
        {
            if(recursive && entry.is_directory())
            {
                const auto attributes = explore(entry);
                print_line(attributes, entry.path().string());
                root_attributes.n_directories++;
            }
            else if(entry.is_directory())
            {
                cout << "           *             ";
                cout << entry.path().string() << endl;
                root_attributes.n_directories++;
            }
            else
            {
                root_attributes.n_files++;
                error_code ec;
                root_attributes.size_bytes += entry.file_size(ec);
                if (ec)
                    cerr << "Error reading file size: " << entry.path().string() << endl;

                cout << setw(12) << entry.file_size() << setw(6) << 1 << setw(6) << " * " << entry.path().string() << endl;
            }
        }
        catch (const exception &)
        {
        }
    }
    print_line(root_attributes, sys_path.string());
}