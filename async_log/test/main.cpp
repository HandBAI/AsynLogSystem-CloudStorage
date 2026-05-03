#include "AsyncLogger.hpp"
#include <iostream>

int main()
{
    std::cout << "Async logger test." << std::endl;
    AsyncLog::AsyncLogger asyncLogger("test");
    asyncLogger.Debug("This is a debug message: %d", 42);
    return 0;
}