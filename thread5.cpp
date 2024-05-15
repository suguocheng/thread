#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <unistd.h>
#include <filesystem>
#include <mutex>

struct SearchConfig {
    std::string root_path;    // 要搜索的根目录
    std::string file_type;    // 要搜索的文件类型，如 ".txt"、".cpp" 等
    int max_concurrency;      // 最大并发数
    int max_depth;            // 最大搜索深度
    bool skip_hidden;         // 是否跳过隐藏文件或目录
    std::vector<std::string> skip_paths;   // 要跳过的目录或文件的路径
} typedef SearchConfig;

std::mutex mtx;
std::vector<std::string> result;

void searchFiles(SearchConfig& searchconfig,std::vector<std::filesystem::path>& directories);
int main()
{
    SearchConfig searchconfig;

    //获取当前路径
    searchconfig.root_path=std::filesystem::current_path();

    //获取当前系统最大线程数
    searchconfig.max_concurrency=std::thread::hardware_concurrency();

    //最大搜索深度
    searchconfig.max_depth=3;

    //要搜索的文件类型
    std::cout << "请输入想要搜索的文件类型：" << std::endl;
    std::cin >> searchconfig.file_type;

    // std::cout << "是否跳过隐藏文件和目录(是则输入1,否则输入0)：" << std::endl;
    // std::cin >> searchconfig.skip_hidden;
    // std::cout << "请输入要跳过的目录或文件的路径(没有则回车)：" << std::endl;
    //  

    //获取的当前路径的目录
    std::vector<std::filesystem::path> directories={searchconfig.root_path};
    for(const auto& entry:std::filesystem::directory_iterator(searchconfig.root_path))
    {
        if(entry.is_directory())
        {
            directories.push_back(entry.path());
        }
    }

    //将目录分配到各线程中
    std::vector<std::vector<std::filesystem::path>> thread_directories(searchconfig.max_concurrency);
    for (size_t i=0;i<directories.size();i++)
    {
        thread_directories[i%searchconfig.max_concurrency].push_back(directories[i]);
    }

    //创建线程
    std::vector<std::thread> threads;
    for(int i=0;i<searchconfig.max_concurrency;i++)
    {
        threads.emplace_back(searchFiles,std::ref(searchconfig),std::ref(thread_directories[i]));
    }
    
    //等待线程结束
    for(auto& thread:threads)
    {
        thread.join();
    }

    //打印结果
    for(const auto& file:result)
    {
        std::cout << file << std::endl;
    }

}
void searchFiles(SearchConfig& searchconfig,std::vector<std::filesystem::path>& directories)
{
    for(const auto& dir:directories)
    {
        for(const auto& entry:std::filesystem::recursive_directory_iterator(dir))
        {
            if(entry.is_regular_file()&&entry.path().extension()==searchconfig.file_type)
            {
                std::lock_guard<std::mutex> lock(mtx);
                result.push_back(entry.path().string());
            }
        }
    }
}