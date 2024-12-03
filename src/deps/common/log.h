
#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <cstdarg>
#include <thread>
#include "single.h"
#include "config.h"
#include "block_queue.h"

namespace common {
typedef enum _LOG_LEVEL {
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    FATAL
}LOG_LEVEL;

class Logger : public Singleton<Logger> {
    std::string m_logFileName;
    std::string m_logFilePath;
    int m_logFileIndex;
    LOG_LEVEL m_logLevel;
    size_t m_maxFileSize;
    size_t m_maxQueueSize;
    size_t m_currentFileSize;
    bool m_outputToConsole;
    bool m_isOpen;
    std::ofstream m_logFileStream;
    std::unique_ptr<BlockQueue<std::string>> m_logQueue;
    std::unique_ptr<std::thread> m_logThread;

    std::mutex m_mutex;
    
    std::string formatLog(LOG_LEVEL logLevel, std::string& file, int line, const std::string& message);
    std::string logLevelToString(LOG_LEVEL logLevel);
    int rotateLogFile();
    void flushLog();
public:
    Logger(): 
     m_logLevel(INFO),
     m_maxFileSize(0), 
     m_currentFileSize(0),
     m_logFileIndex(0),
     m_maxQueueSize(0),
     m_outputToConsole(true),
     m_isOpen(true)
     {};
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    void init();
    int setLogFile(std::string filename);
    void setLogLevel(LOG_LEVEL logLevel);
    void setMaxFileSize(size_t maxFileSize);
    void log(LOG_LEVEL logLevel, const std::string &file, int line, const std::string& format, ...);

};

}

#define LOG_DEBUG(format, ...) common::Logger::getInstance().log(common::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__ )
#define LOG_INFO(format, ...) common::Logger::getInstance().log(common::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__ )
#define LOG_WARN(format, ...) common::Logger::getInstance().log(common::WARNING, __FILE__, __LINE__, format, ##__VA_ARGS__ )
#define LOG_ERROR(format, ...) common::Logger::getInstance().log(common::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__ )
