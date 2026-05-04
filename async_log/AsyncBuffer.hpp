/**
 * @file AsyncBuffer.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2026-05-04
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef ASYNC_BUFFER
#define ASYNC_BUFFER
#include "Utils.hpp"
#include <cassert>
#include <cstring>
#include <string>
#include <vector>
namespace AsyncLog {
class AsyncBuffer {
public:
    AsyncBuffer() : m_buffer(std::vector<char>(1024 * 1024)) {}
    // push
    void Push(const char *data, size_t len)
    {
        auto size = m_buffer.size();
        if (len > LeftSize()) {
            std::cout << "Left space is not enough." << std::endl;
            return;
        }
        memcpy(m_buffer.data() + m_writePos, data, len);
        m_writePos += len;
    }
    void Swap(AsyncBuffer &buf)
    {
        m_buffer.swap(buf.m_buffer);
        std::swap(m_writePos, buf.m_writePos);
    }
    void Reset()
    {
        m_writePos = 0;
    }
    bool Empty()
    {
        return m_writePos == 0;
    }
    size_t LeftSize()
    {
        return m_buffer.size() - m_writePos;
    }
    size_t Size()
    {
        return m_writePos;
    }
    std::vector<char> m_buffer;

private:
    size_t m_writePos{0};
};
} // namespace AsyncLog
#endif