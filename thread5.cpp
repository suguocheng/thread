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

void searchFiles(SearchConfig *searchconfig);
int main()
{
    SearchConfig searchconfig;
    char buf[100];
    getcwd(buf,sizeof(buf));
    searchconfig.root_path=buf;
    searchconfig.max_concurrency=std::thread::hardware_concurrency();;
    searchconfig.max_depth=3;
    std::cout << "请输入想要搜索的文件类型：" << std::endl;
    std::cin >> searchconfig.file_type;
    // std::cout << "是否跳过隐藏文件和目录(是则输入1,否则输入0)：" << std::endl;
    // std::cin >> searchconfig.skip_hidden;
    // std::cout << "请输入要跳过的目录或文件的路径(没有则回车)：" << std::endl;
    // std::cin 
    std::thread thread1(searchFiles,&searchconfig);
    std::thread thread2(searchFiles,&searchconfig);

    thread1.join();
    thread2.join();
}
void searchFiles(SearchConfig *searchconfig)
{
    std::filesystem::path directory_path=searchconfig->root_path;
    for(const auto& entry : std::filesystem::directory_iterator(directory_path))
    {
        // mtx.lock();
        if (std::filesystem::is_regular_file(entry)&&entry.path().extension()==searchconfig->file_type)
        {
            std::cout << entry.path() << std::endl;
        }
        else if (std::filesystem::is_directory(entry))
        {
            searchconfig->root_path=entry.path();
            searchFiles(searchconfig); // 递归搜索子目录
        }
        // mtx.unlock();
    }
}