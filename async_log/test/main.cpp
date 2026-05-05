#include "AsyncLogger.hpp"
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
    auto logger = LoggerBuilder.Build("test", AsyncLog::AsyncType::ASYNC_SAFE, {consolLogFlush});

    for (size_t i = 0; i < 1024; ++i) {
        logger->Debug("This is a log file test message:%d.", i);
    }

    return 0;
}