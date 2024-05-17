#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <filesystem>
#include <mutex>
#include <algorithm>

struct SearchConfig
{
    std::string root_path;               // 要搜索的根目录
    std::string file_type;               // 要搜索的文件类型，如 ".txt"、".cpp" 等
    int max_concurrency;                 // 最大并发数
    int max_depth;                       // 最大搜索深度
    bool skip_hidden;                    // 是否跳过隐藏文件或目录
    std::vector<std::string> skip_paths; // 要跳过的目录或文件的路径
} typedef SearchConfig;

std::mutex mtx;
std::vector<std::string> result;

void searchFiles(const SearchConfig &searchconfig, const std::filesystem::path &dir, int current_depth);
int main()
{
    SearchConfig searchconfig;

    // 获取当前路径
    searchconfig.root_path = std::filesystem::current_path().string();

    // 获取当前系统最大线程数
    searchconfig.max_concurrency = std::thread::hardware_concurrency();

    // 最大搜索深度
    searchconfig.max_depth = 1;

    // 要搜索的文件类型
    std::cout << "请输入想要搜索的文件类型：" << std::endl;
    std::cin >> searchconfig.file_type;

    // 是否跳过隐藏文件和目录
    std::cout << "是否跳过隐藏文件和目录(是则输入1,否则输入0): " << std::endl;
    std::cin >> searchconfig.skip_hidden;

    // 要跳过的目录或文件的路径
    std::cout << "请输入要跳过的目录或文件的路径(输入完按回车，结束输入按Ctrl+D车)：" << std::endl;
    std::string skip_path;
    while (std::cin >> skip_path)
    {
        searchconfig.skip_paths.push_back(skip_path);
    }

    // 清除输入错误标志
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 获取的当前路径的目录
    std::vector<std::filesystem::path> directories = {searchconfig.root_path};
    for (const auto &entry : std::filesystem::directory_iterator(searchconfig.root_path))
    {
        if (entry.is_directory())
        {
            directories.push_back(entry.path());
        }
    }

    // 将目录分配到各线程中
    std::vector<std::vector<std::filesystem::path>> thread_directories(searchconfig.max_concurrency);
    for (size_t i = 0; i < directories.size(); i++)
    {
        thread_directories[i % searchconfig.max_concurrency].push_back(directories[i]);
    }

    // 创建线程
    std::vector<std::thread> threads;
    // for (int i = 0; i < searchconfig.max_concurrency; i++)
    // {
    //     threads.emplace_back([&, i]()
    //                          {
    //         for (const auto& dir : thread_directories[i]) {
    //             searchFiles(searchconfig, dir, 0);
    //         } });
    // }
    for (int i = 0; i < searchconfig.max_concurrency; i++)
    {
        for (const auto &dir : thread_directories[i])
        {
            threads.emplace_back(&searchFiles, std::ref(searchconfig), std::ref(dir), 0);
        }
    }

    // 等待线程结束
    for (auto &thread : threads)
    {
        thread.join();
    }

    // 打印结果
    std::cout << "搜索到的文件为：" << std::endl;
    for (const auto &file : result)
    {
        std::cout << file << std::endl;
    }

    return 0;
}
void searchFiles(const SearchConfig &searchconfig, const std::filesystem::path &dir, int current_depth)
{
    if (current_depth >= searchconfig.max_depth)
    {
        return;
    }

    for (const auto &entry : std::filesystem::directory_iterator(dir))
    {
        // 跳过隐藏文件和目录
        if (searchconfig.skip_hidden && entry.path().filename().string()[0] == '.')
        {
            continue;
        }

        // 跳过指定路径
        if (std::find(searchconfig.skip_paths.begin(), searchconfig.skip_paths.end(), entry.path().string()) != searchconfig.skip_paths.end())
        {
            continue;
        }

        if (entry.is_directory())
        {
            searchFiles(searchconfig, entry.path(), current_depth + 1);
        }
        else if (entry.is_regular_file() && entry.path().extension() == searchconfig.file_type)
        {
            std::lock_guard<std::mutex> lock(mtx);
            result.push_back(entry.path().string());
        }
    }
}