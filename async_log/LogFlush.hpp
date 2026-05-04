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
namespace AsyncLog {

class LogFlush {
public:
    LogFlush() = default;
    virtual ~LogFlush() = default;
    virtual void Flush(const char *msg, size_t len) = 0;
};

class ConsolLogFlush : public LogFlush {
public:
    ConsolLogFlush() = default;
    virtual ~ConsolLogFlush() = default;
    void Flush(const char *msg, size_t len) override
    {
        std::cout.write(msg, len);
    }
};
// 输出到文件:创建文件,写入日志
class FileLogFlush : public LogFlush {
public:
    FileLogFlush(const std::string &fileName, size_t maxSize)
        : m_fileName(fileName), m_maxSize(maxSize)
    {

        Utils::File::CreateParentDirectory(fileName);
        m_dirFileName = Utils::File::GetDirAndFileName(fileName);

        m_fs = fopen(fileName.c_str(), "ab");
        if (m_fs == nullptr) {
            std::cerr << __FILE__ << ":" << __LINE__ << " ";
            perror("open log file failed");
            return;
        }
    };

    virtual ~FileLogFlush()
    {
        if (m_fs) {
            fclose(m_fs);
            m_fs = nullptr;
        }
    };

    void Flush(const char *msg, size_t len) override
    {
        if (m_fs == nullptr) {
            perror("open log file failed");
            return;
        }
        auto m_curSize = Utils::File::GetFileSize(m_fileName);
        if (m_curSize == -1)
            return;
        if (m_curSize + len > m_maxSize) {
            if (RotateLog() == -1) {
                return;
            }
        }

        fwrite(msg, 1, len, m_fs);
        if (ferror(m_fs)) {
            std::cout << __FILE__ << __LINE__ << "write log file failed"
                      << std::endl;
            perror(NULL);
        }
    }

private:
    int RotateLog()
    {
        if (m_fs) {
            fclose(m_fs);
            m_fs = nullptr;
        }
        // 重命名日志
        auto newFileName = m_dirFileName.first + "/" + m_dirFileName.second + "_" + Utils::Time::GetCurTime() + ".log";

        auto ret = rename(m_fileName.c_str(), newFileName.c_str());
        if (ret == -1) {
            perror("Rename log file name Failed.");
            return ret;
        }

        m_fs = fopen(m_fileName.c_str(), "ab");
        if (m_fs == nullptr) {
            return -1;
        }
        return 0;
    }
    std::string m_fileName;
    std::pair<std::string, std::string> m_dirFileName;
    FILE *m_fs = NULL;
    size_t m_curSize = 0;
    size_t m_maxSize;
};
} // namespace AsyncLog
#endif