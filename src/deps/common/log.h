
#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <cstdarg>


typedef enum _LOG_LEVEL {
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    FATAL
}LOG_LEVEL;

class Logger {
    std::string m_logFileName;
    std::string m_logFilePath;
    int m_logFileIndex;
    LOG_LEVEL m_logLevel;
    size_t m_maxFileSize;
    size_t m_currentFileSize;
    std::ofstream m_logFileStream;

    std::mutex m_mutex;
    
    std::string formatLog(LOG_LEVEL logLevel, std::string& file, int line, const std::string& message);
    std::string logLevelToString(LOG_LEVEL logLevel);
    int rotateLogFile();
public:
    Logger(): 
     m_logLevel(INFO),
     m_maxFileSize(0), 
     m_currentFileSize(0),
     m_logFileIndex(0) 
     {};
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    static Logger& getInstance();
    int setLogFile(std::string &filename);
    void setLogLevel(LOG_LEVEL logLevel);
    void setMaxFileSize(size_t maxFileSize);
    void log(LOG_LEVEL logLevel, const std::string &file, int line, const std::string& format, ...);

};

#define LOG_DEBUG(format, ...) Logger::getInstance().log(DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__ )
#define LOG_INFO(format, ...) Logger::getInstance().log(INFO, __FILE__, __LINE__, format, ##__VA_ARGS__ )
#define LOG_WARN(format, ...) Logger::getInstance().log(WARNING, __FILE__, __LINE__, format, ##__VA_ARGS__ )
#define LOG_ERROR(format, ...) Logger::getInstance().log(ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__ )
