/**
 * @file Utils.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2026-05-04
 *
 * @copyright Copyright (c) 2026
 *
 */
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
namespace AsyncLog::Utils
{

class File
{
public:
    static std::string Path(const std::string &filename)
    {
        if (filename.empty())
            return "";
        int pos = filename.find_last_of("/\\");
        if (pos != std::string::npos)
            return filename.substr(0, pos + 1);
        return "";
    }
    // 辅助函数：判断文件/目录是否存在
    static bool Exists(const std::string &path)
    {
        struct stat info{};
        return stat(path.c_str(), &info) == 0;
    }

    static void CreateParentDirectory(const std::string &filePath)
    {
        size_t pos = filePath.find_last_of("/\\");
        if (pos != std::string::npos) {
            std::string dir = filePath.substr(0, pos);
            CreateDirectory(dir);
        }
        std::cout << "No directory is needed to be created." << std::endl;
    }

    // 递归创建多级目录（兼容 Linux/macOS）
    static void CreateDirectory(const std::string &path)
    {
        if (Exists(path)) {
            std::cout << "director is exist: " << path << std::endl;
            return;
        }

        size_t pos = path.find_last_of("/\\");
        if (pos != std::string::npos) {
            // 截取父目录并递归创建
            std::string parent_path = path.substr(0, pos);
            CreateDirectory(parent_path);
        }

        if (mkdir(path.c_str(), 0755) == -1) {
            perror(("Create director failed:" + path).c_str());
        }
    }

    int64_t FileSize(std::string filename)
    {
        struct stat s;
        auto ret = stat(filename.c_str(), &s);
        if (ret == -1) {
            perror("Get file size failed");
            return -1;
        }
        return s.st_size;
    }
    // 获取文件内容
    bool GetContent(std::string *content, std::string filename)
    {
        // 打开文件
        std::ifstream ifs;
        ifs.open(filename.c_str(), std::ios::binary);
        if (ifs.is_open() == false) {
            std::cout << "file open error" << std::endl;
            return false;
        }

        // 读入content
        ifs.seekg(0, std::ios::beg); // 更改文件指针的偏移量
        size_t len = FileSize(filename);
        content->resize(len);
        ifs.read(&(*content)[0], len);
        if (!ifs.good()) {
            std::cout << __FILE__ << __LINE__ << "-"
                      << "read file content error" << std::endl;
            ifs.close();
            return false;
        }
        ifs.close();

        return true;
    }
}; // class file

} // namespace AsyncLog::Utils