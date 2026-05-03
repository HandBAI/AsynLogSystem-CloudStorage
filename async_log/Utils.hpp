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
    static bool Exists(const std::string &filename)
    {
        struct stat st;
        return (0 == stat(filename.c_str(), &st));
    }
    static std::string Path(const std::string &filename)
    {
        if (filename.empty())
            return "";
        int pos = filename.find_last_of("/\\");
        if (pos != std::string::npos)
            return filename.substr(0, pos + 1);
        return "";
    }
    static void CreateDirectory(const std::string &pathname)
    {
        if (pathname.empty())
            perror("文件所给路径为空：");
        // 文件不存在再创建
        if (!Exists(pathname)) {
            size_t pos, index = 0;
            size_t size = pathname.size();
            while (index < size) {
                pos = pathname.find_first_of("/", index);
                if (pos == std::string::npos) {
                    mkdir(pathname.c_str(), 0755);
                    return;
                }
                if (pos == index) {
                    index = pos + 1;
                    continue;
                }

                std::string sub_path = pathname.substr(0, pos);
                if (sub_path == "." || sub_path == "..") {
                    index = pos + 1;
                    continue;
                }
                if (Exists(sub_path)) {
                    index = pos + 1;
                    continue;
                }

                mkdir(sub_path.c_str(), 0755);
                index = pos + 1;
            }
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

} // namespace AsyncLog