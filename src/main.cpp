#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

void copy_directory(const fs::path& sourceDir, const fs::path& destDir)
{
    if (!fs::exists(sourceDir) || !fs::is_directory(sourceDir))
    {
        throw std::runtime_error("Source directory does not exist or is not a directory.");
    }

    if (!fs::exists(destDir))
    {
        fs::create_directories(destDir);
    }

    for (const auto& entry : fs::directory_iterator(sourceDir))
    {
        const auto& path = entry.path();
        auto dest = destDir / path.filename();

        if (fs::is_directory(path))
        {
            copy_directory(path, dest);
        }

        else if (fs::is_regular_file(path))
        {
            if (!fs::exists(dest))
            {
                fs::copy_file(path, dest, fs::copy_options::skip_existing);
                std::cout << "Copied: " << std::endl;
            }
            else
            {
                std::cout << "File done, Skipping: " << std::endl;
            }
        }
    }
}

int main()
{
    std::string source = "C://Users/gorkh/OneDrive/Desktop/A";
    std::string destination = "C://Users/gorkh/OneDrive/Desktop/B";

    while (true)
    {
        try
        {
            copy_directory(source, destination);
            std::cout << "Files copied successfully!" << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}