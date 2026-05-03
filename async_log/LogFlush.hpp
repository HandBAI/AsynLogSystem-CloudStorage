/**
 * @file LogFlush.hpp
 * @author your name (you@domain.com)
 * @brief 日志输出控制
 * @version 0.1
 * @date 2026-05-04
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef LOG_FLUSH_HPP
#define LOG_FLUSH_HPP
#include "Utils.hpp"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
namespace AsyncLog
{

class LogFlush
{
public:
    LogFlush() = default;
    virtual ~LogFlush() = default;
    virtual void Flush(const char *msg, size_t len) = 0;
};

class ConsolLogFlush : public LogFlush
{
public:
    ConsolLogFlush() = default;
    virtual ~ConsolLogFlush() = default;
    void Flush(const char *msg, size_t len) override
    {
        std::cout.write(msg, len);
    }
};
// 输出到文件:创建文件,写入日志
class FileLogFlush : public LogFlush
{
public:
    FileLogFlush(const std::string &fileName) : m_fileName(fileName)
    {
        // 创建目录 打开文件
        Utils::File::CreateDirectory(fileName);
        m_fs = fopen(fileName.c_str(), "ab");
        if (m_fs == NULL) {
            std::cout << __FILE__ << __LINE__ << "open log file failed"
                      << std::endl;
            perror(NULL);
        }
    };
    virtual ~FileLogFlush() = default;
    void Flush(const char *msg, size_t len) override
    {
        fwrite(msg, 1, len, m_fs);
        if (ferror(m_fs)) {
            std::cout << __FILE__ << __LINE__ << "write log file failed"
                      << std::endl;
            perror(NULL);
        }
        // if (g_conf_msg->flush_log == 1) {
        //     if (fflush(m_fs) == EOF) {
        //         std::cout << __FILE__ << __LINE__ << "fflush file failed"
        //                   << std::endl;
        //         perror(NULL);
        //     }
        // } else if (g_conf_msg->flush_log == 2) {
        //     fflush(m_fs);
        //     fsync(fileno(m_fs));
        // }
    }

private:
    std::string m_fileName;
    FILE *m_fs = NULL;
};
} // namespace AsyncLog
#endif