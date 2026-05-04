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
    AsyncLog::AsyncLogger asyncLogger("test", fileLogFlush);

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 10000; ++i) {
        asyncLogger.Debug("This is a log file test message:%d.", i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "End time:" << ms << std::endl;
    return 0;
}