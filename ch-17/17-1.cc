#include <iostream>
#include <filesystem>

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cerr << "Usage: main.out PATH EXT";
        return -1;
    }

    std::filesystem::path path{argv[1]};
    std::string extension{argv[2]};
    if(extension[0] != '.') extension = "." + extension;

    for(const auto& entry : std::filesystem::recursive_directory_iterator{
        path, 
        std::filesystem::directory_options::skip_permission_denied} )
    {
        if(entry.path().extension() == extension)
        {
            std::cout << entry.path() << std::endl;
        }
    }

    return 0;
}