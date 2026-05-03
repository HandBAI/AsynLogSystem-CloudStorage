#include "AsyncLogger.hpp"
#include <iostream>

int main()
{
    std::cout << "Async logger test." << std::endl;

    auto fileLogFlush = std::make_shared<AsyncLog::FileLogFlush>("./test.log");
    auto consolLogFlush = std::make_shared<AsyncLog::ConsolLogFlush>();
    AsyncLog::AsyncLogger asyncLogger("test", fileLogFlush);

    asyncLogger.Debug("This is a debug message: %d", 42);
    return 0;
}