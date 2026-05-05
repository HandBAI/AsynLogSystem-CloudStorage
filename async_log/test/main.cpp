#include "AsyncLogger.hpp"
#include "AsyncLoggerManage.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <time.h>
constexpr size_t MAX_SIZE_TEST = 1024 * 1024; // 1k
int main()
{
    std::cout << "Async logger test." << std::endl;

    auto fileLogFlush = std::make_shared<AsyncLog::FileLogFlush>("./test_log/test1.log", MAX_SIZE_TEST);
    auto consolLogFlush = std::make_shared<AsyncLog::ConsolLogFlush>();
    AsyncLog::LoggerBuilder LoggerBuilder;
    std::string loggerTest = "test";
    auto logger = LoggerBuilder.Build(loggerTest, AsyncLog::AsyncType::ASYNC_SAFE, {consolLogFlush});
    AsyncLog::AsyncLoggerManage m_asyncLoggerMange;
    m_asyncLoggerMange.AddLogger(std::move(logger));

    for (size_t i = 0; i < 1024; ++i) {
        m_asyncLoggerMange.GetLogger(loggerTest)->Debug("This is a log file test message:%d.", i);
    }

    return 0;
}