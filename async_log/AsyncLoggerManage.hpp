/**
 * @file AsyncLoggerManage.hpp
 * @author your name (you@domain.com)
 * @brief 管理单例，负责logger的创建, name
 * @version 0.1
 * @date 2026-05-05
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef ASYNC_LOGGER_MANAGE
#define ASYNC_LOGGER_MANAGE
#include "AsyncLogger.hpp"
#include <shared_mutex>
#include <unordered_map>
namespace AsyncLog {
class AsyncLoggerManage {

public:
    AsyncLoggerManage(/* args */)
    {
        m_loggerBuilder = std::make_unique<LoggerBuilder>();
        m_defaultLogger = m_loggerBuilder->Build("defalt", AsyncType::ASYNC_SAFE, {});
        m_loggerMap.emplace(m_defaultLogger->Name(), m_defaultLogger);
    };
    ~AsyncLoggerManage() = default;
    static AsyncLoggerManage &Instance()
    {
        static AsyncLoggerManage manage;
        return manage;
    }

    bool LoggerExist(const std::string &name)
    {
        std::shared_lock<std::shared_mutex> rlock(m_mtx);
        auto it = m_loggerMap.find(name);
        if (it == m_loggerMap.end())
            return false;
        return true;
    }

    void AddLogger(const std::shared_ptr<AsyncLogger> &&logger)
    {
        if (LoggerExist(logger->Name())) {
            return;
        }
        std::unique_lock<std::shared_mutex> wlock(m_mtx);
        m_loggerMap.emplace(logger->Name(), logger);
    }

    std::shared_ptr<AsyncLogger> GetLogger(const std::string &name)
    {
        if (!LoggerExist(name)) {
            return m_loggerBuilder->Build(name, AsyncType::ASYNC_SAFE, {}); // 后续完善
        }
        std::shared_lock<std::shared_mutex> rlock(m_mtx);
        return m_loggerMap.find(name)->second;
    }

private:
    std::unique_ptr<LoggerBuilder> m_loggerBuilder;
    std::shared_ptr<AsyncLogger> m_defaultLogger;
    std::shared_mutex m_mtx;
    std::unordered_map<std::string, std::shared_ptr<AsyncLogger>> m_loggerMap;
};
} // namespace AsyncLog

#endif