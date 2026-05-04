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
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#include <cstdio>
#include <iomanip>
#include <sstream>
#ifndef UTILS
#define UTILS
namespace AsyncLog::Utils {
class Time {
private:
    /* data */
public:
    Time(/* args */) = default;
    ~Time() = default;
    static std::string GetCurrentTimeStamp(int time_stamp_type = 0)
    {
        std::chrono::system_clock::time_point now =
            std::chrono::system_clock::now();

        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm *now_tm = std::localtime(&now_time_t);

        char buffer[128];
        strftime(buffer, sizeof(buffer), "%F_%T", now_tm);

        std::ostringstream ss;
        ss.fill('0');

        std::chrono::milliseconds ms;
        std::chrono::microseconds cs;
        std::chrono::nanoseconds ns;

        switch (time_stamp_type) {
        case 0:
            ss << buffer;
            break;
        case 1:
            ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
            ss << buffer << ":" << ms.count();
            break;
        case 2:
            ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
            cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
            ss << buffer << ":" << ms.count() << ":" << cs.count() % 1000;
            break;
        case 3:
            ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
            cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
            ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()) % 1000000000;
            ss << buffer << ":" << ms.count() << ":" << cs.count() % 1000 << ":" << ns.count() % 1000;
            break;
        default:
            ss << buffer;
            break;
        }

        return ss.str();
    }

    static std::string GetCurTime() { return GetCurrentTimeStamp(2); }
};

class File {
public:
    // 辅助函数：判断文件/目录是否存在
    static bool Exists(const std::string &path)
    {
        struct stat info{};
        return stat(path.c_str(), &info) == 0;
    }

    static std::pair<std::string, std::string> GetDirAndFileName(const std::string &filePath)
    {
        size_t pos = filePath.find_last_of("/\\");
        if (pos == std::string::npos) {
            return std::pair("", "");
        }
        auto dir = filePath.substr(0, pos);
        auto file = filePath.substr(pos + 1, std::string::npos);
        pos = file.find_last_of('.');
        if (pos == std::string::npos) {
            return std::pair(dir, "");
        }
        return std::pair(dir, file.substr(0, pos));
    }

    static void CreateParentDirectory(const std::string &filePath)
    {
        size_t pos = filePath.find_last_of("/\\");
        if (pos != std::string::npos) {
            std::string dir = filePath.substr(0, pos);
            CreateDirectory(dir);
        }
    }

    // 递归创建多级目录（兼容 Linux/macOS）
    static void CreateDirectory(const std::string &path)
    {
        if (Exists(path)) {
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

    static int64_t GetFileSize(std::string filename)
    {
        struct stat s;
        auto ret = stat(filename.c_str(), &s);
        if (ret == -1) {
            perror("Get file size failed");
            return -1;
        }
        return s.st_size;
    }
}; // class file

} // namespace AsyncLog::Utils
#endif