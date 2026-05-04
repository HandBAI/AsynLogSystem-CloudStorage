/**
 * @file AsyncWorker.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2026-05-04
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef ASYNC_WORKER
#define ASYNC_WORKER
#include "AsyncBuffer.hpp"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
namespace AsyncLog {
// 构造时起工作线程, 调用回调刷写数据
// push接收日志buf, 通知工作线程刷新
enum class AsyncType {
    ASYNC_SAFE,
    ASYNC_UNSAFE
};
using AsyncWorkFlushCallBack = std::function<void(const char *data, size_t size)>;
class AsyncWorker {
public:
    AsyncWorker(AsyncType type, const AsyncWorkFlushCallBack &cb) : m_asyncType(type),
                                                                    m_flushCallBack(cb),
                                                                    m_thread(std::thread(&AsyncWorker::AsyncWorkerThread, this))
    {
        m_start = std::chrono::high_resolution_clock::now();
    }
    ~AsyncWorker()
    {
        // std::cout << "~AsyncWorker" << std::endl;
        m_stop = true;
        m_comsumerVar.notify_one();
        m_thread.join();
        auto end = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count();
        std::cout << "End time:" << ms << std::endl;
    }

    // 生产者
    void Push(const char *data, size_t size)
    {
        std::unique_lock<std::mutex> lock;
        if (m_asyncType == AsyncType::ASYNC_SAFE) {
            // 空间不足等待
            // std::cout << "[" << std::this_thread::get_id() << "] size:" << size << ", left:" << m_productorBuf.LeftSize() << std::endl;
            m_productorVar.wait(lock, [&]() {
                return size <= m_productorBuf.LeftSize();
            });
        }
        m_productorBuf.Push(data, size);
        m_comsumerVar.notify_one();
    }

private:
    void AsyncWorkerThread()
    {
        while (!m_stop || !m_productorBuf.Empty()) {
            {
                std::unique_lock<std::mutex> lock(m_mutx);
                m_comsumerVar.wait(lock, [&]() {
                    return m_stop || !m_productorBuf.Empty();
                });
                m_productorBuf.Swap(m_comsumerBuf);
                if (m_asyncType == AsyncType::ASYNC_SAFE) {
                    m_productorVar.notify_one();
                }
            }
            // 回调消费消费者数据
            // std::cout << "[" << std::this_thread::get_id() << "] consum" << std::endl;
            m_flushCallBack(m_comsumerBuf.m_buffer.data(), m_comsumerBuf.Size());
            m_comsumerBuf.Reset();
        }
    }
    AsyncWorkFlushCallBack m_flushCallBack;
    AsyncType m_asyncType{AsyncType::ASYNC_SAFE};
    std::atomic<bool> m_stop{false};
    std::mutex m_mutx;
    AsyncBuffer m_productorBuf;
    AsyncBuffer m_comsumerBuf;
    std::condition_variable m_productorVar;
    std::condition_variable m_comsumerVar;
    std::thread m_thread;
    std::chrono::high_resolution_clock::time_point m_start;
    std::chrono::high_resolution_clock::time_point m_end;
};
} // namespace AsyncLog
#endif