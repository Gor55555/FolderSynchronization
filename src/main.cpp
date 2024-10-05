#include <iostream>
#include <filesystem>
#include <string>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

void FolderSynchronization(const fs::path& FolderA, const fs::path& FolderB)
{

    if (!fs::exists(FolderA) || !fs::is_directory(FolderB))
    {
        throw std::runtime_error("Folder_A: doesn't exist");
    }

    if (!fs::exists(FolderB))
    {
        throw std::runtime_error("Folder_B: doesn't exist");
    }

    for (const auto& entry : fs::directory_iterator(FolderA))
    {
        const auto& sourcePath = entry.path();
        auto destinationPath = FolderB / sourcePath.filename();

        if (fs::is_directory(sourcePath))
        {
            if (!fs::exists(destinationPath))
            {
                fs::create_directories(destinationPath);
            }
            FolderSynchronization(sourcePath, destinationPath);
        }
        else if (fs::is_regular_file(sourcePath))
        {
            if (!fs::exists(destinationPath))
            {
                fs::copy_file(sourcePath, destinationPath, fs::copy_options::skip_existing);
            }
        }
    }

    for (const auto& entry : fs::directory_iterator(FolderB))
    {
        const auto& destinationPath = entry.path();
        auto sourcePath = FolderA / destinationPath.filename();

        if (!fs::exists(sourcePath))
        {
            if (fs::is_directory(destinationPath))
            {
                fs::remove_all(destinationPath);
            }
            else if (fs::is_regular_file(destinationPath))
            {
                fs::remove(destinationPath);
            }
        }
    }
}

int main()
{
    std::string source = "C://Users/gorkh/OneDrive/Desktop/A";
    std::string destination = "C://Users/gorkh/OneDrive/Desktop/B";

    for (;;)
    {
        try
        {
            FolderSynchronization(source, destination);
            std::cerr << "SYnchronization" << "\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << "\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}