/**
 * @file AsyncLogger.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2026-05-03
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef ASYNC_LOGGER_HPP
#define ASYNC_LOGGER_HPP
#include "LogFlush.hpp"
#include <cstdarg>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdio.h>
#include <string>
#include <thread>
#include <vector>
// 日志解析
// 日志写入

namespace AsyncLog {

#define Debug(fmt, ...) AsyncLogFlush(__FILE__, __LINE__, AsyncLog::LogLevel::value::DEBUG, fmt, ##__VA_ARGS__)
#define Info(fmt, ...) AsyncLogFlush(__FILE__, __LINE__, AsyncLog::LogLevel::value::INFO, fmt, ##__VA_ARGS__)
#define Warn(fmt, ...) AsyncLogFlush(__FILE__, __LINE__, AsyncLog::LogLevel::value::WARN, fmt, ##__VA_ARGS__)
#define Error(fmt, ...) AsyncLogFlush(__FILE__, __LINE__, AsyncLog::LogLevel::value::ERROR, fmt, ##__VA_ARGS__)
#define Fatal(fmt, ...) AsyncLogFlush(__FILE__, __LINE__, AsyncLog::LogLevel::value::FATAL, fmt, ##__VA_ARGS__)

class LogLevel {
public:
    enum class value { DEBUG,
                       INFO,
                       WARN,
                       ERROR,
                       FATAL };

    // 提供日志等级的字符串转换接口
    static const char *ToString(value level)
    {
        switch (level) {
        case value::DEBUG:
            return "DEBUG";
        case value::INFO:
            return "INFO";
        case value::WARN:
            return "WARN";
        case value::ERROR:
            return "ERROR";
        case value::FATAL:
            return "FATAL";
        default:
            return "UNKNOW";
        }
        return "UNKNOW";
    }
};

struct logMassage {
    logMassage(std::string fileName, size_t line, std::string loggerName,
               LogLevel::value level, std::string payLoad)
        : m_fileName(fileName), m_line(line), m_level(level),
          m_payLoad(payLoad), m_threadId(std::this_thread::get_id()),
          m_ctime(time(nullptr))
    {
    }
    std::string format()
    {
        std::ostringstream oss;
        // 进程id+时间+等级+文件名+行号+信息
        oss << "[" << m_threadId << "][" << Utils::Time::GetCurTime() << "]["
            << LogLevel::ToString(m_level) << "][" << m_fileName << ":"
            << m_line << "]" << m_payLoad << "\n";
        return oss.str();
    }
    std::string m_fileName;
    size_t m_line;
    std::string m_loggerName;
    LogLevel::value m_level;
    std::string m_payLoad;
    std::thread::id m_threadId;
    time_t m_ctime;
};

class AsyncLogger {
public:
    AsyncLogger(std::string logFileName, const std::vector<std::shared_ptr<LogFlush>> &logFlush)
        : m_logFileName(logFileName), m_logFlush(logFlush) {};
    ~AsyncLogger() = default;
    void AsyncLogFlush(const char *file, size_t line, LogLevel::value level, const char *fmt, ...)
    {
        // 打印输入参数
        va_list va;
        va_start(va, fmt);
        char *msg;
        // 获取可变参数
        auto result = vasprintf(&msg, fmt, va);
        if (result == -1) {
            perror("vasprintf failed!!!: ");
        }
        Serialze(file, line, level, msg);
        va_end(va);
        free(msg);
    }

private:
    void Serialze(const std::string &fileName, size_t line, LogLevel::value level, char *msg)
    {
        // 组装日志信息, 添加时间, 等级，进程ID
        logMassage log(fileName, line, "", level, msg);
        auto str = log.format();
        for (const auto &flush : m_logFlush) {
            flush->Flush(str.c_str(), str.length());
        }
    }
    // 内部实现细节
    std::string m_logFileName;
    std::vector<std::shared_ptr<LogFlush>> m_logFlush;
};

} // namespace AsyncLog
#endif // ASYNC_LOGGER_HPP