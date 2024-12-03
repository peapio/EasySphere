#include "log.h"
#include "utils.h"

namespace common {

Logger::~Logger()
{
    if (m_logQueue && m_logThread && m_logThread->joinable()) {
        m_isOpen = false;
        m_logThread->join();
    }
    m_logFileStream.flush();
    if(m_logFileStream.is_open())
    {
        m_logFileStream.close();
    }
}

void Logger::init() {
    setLogFile(gConf.conf("trace_file"));
    setMaxFileSize(std::stoul(gConf.conf("maximum_trace_file_size")));
    m_maxQueueSize = std::stoi(gConf.conf("max_queue_size"));
    if (m_maxQueueSize > 0) {
        m_logQueue = std::make_unique<BlockQueue<std::string>>(m_maxQueueSize);
        m_logThread = std::make_unique<std::thread>(&Logger::flushLog, this);
    }
}

void Logger::flushLog() {
    while (m_isOpen || !m_logQueue->empty()) {
        std::string log;
        m_logQueue->pop(log);
        m_logFileStream << log << std::endl;
    }
}

int Logger::setLogFile(std::string filename)
{
    if(m_logFileStream.is_open())
    {
        m_logFileStream.close();
    }
    size_t index = filename.find_last_of("/");
    if(index == std::string::npos)
    {
        return -1;
    }

    m_logFilePath = filename.substr(0, index + 1);
    m_logFileName = filename.substr(index + 1, filename.size() - index - 1);
    if(m_logFileName.rfind(".") != std::string::npos)
    {
        m_logFileName = m_logFileName.substr(0, m_logFileName.rfind("."));
    }
    filename = m_logFilePath + m_logFileName + "_" + getTimeStamp("%Y-%m-%d") + ".log";
    m_logFileStream.open(filename, std::ios::out | std::ios::app);
    if(!m_logFileStream.is_open())
    {
        return errno;
    }
    return 0;
}

void Logger::setLogLevel(LOG_LEVEL logLevel)
{
    m_logLevel = logLevel;
}

void Logger::setMaxFileSize(size_t maxFileSize)
{
    m_maxFileSize = maxFileSize;
}

void Logger::log(LOG_LEVEL logLevel,const std::string& file, int line, const std::string& format, ...)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (logLevel >= m_logLevel)
    {
        va_list args;
        va_start(args, format);
        char message[1024];
        vsnprintf(message, sizeof(message), format.c_str(), args);
        va_end(args);
        std::string file_str = file;
        std::string logMessage = formatLog(logLevel, file_str, line, message);
        m_currentFileSize += logMessage.size();

        if(m_currentFileSize > m_maxFileSize && m_maxFileSize > 0)
        {
            rotateLogFile();
        }
        if (m_maxQueueSize > 0 && m_logQueue)
        {
            m_logQueue->push(logMessage);
        }
        else 
        {
            if (m_outputToConsole)
            {
                std::cout << logMessage << std::endl;
            }
            if(m_logFileStream.is_open())
            {
                m_logFileStream << logMessage << std::endl;
            }
            else {
                std::cerr << "log file is not open" << std::endl;
            }
        }
    }
}

int Logger::rotateLogFile()
{
    if(m_logFileStream.is_open())
    {
        m_logFileStream.close();
    }
    std::string newFileName = m_logFileName + "_" + std::to_string(++m_logFileIndex) + "_" + getTimeStamp("%Y-%m-%d") + ".log";
    std::string newLogFile = m_logFilePath + newFileName;
    m_currentFileSize = 0;
    m_logFileStream.open(newLogFile, std::ios::out | std::ios::trunc);
    if(!m_logFileStream.is_open())
    {
        return errno;
    }
    return 0;
}

std::string Logger::formatLog(LOG_LEVEL logLevel, std::string& file, int line, const std::string& message)
{
    std::string logLevelString = logLevelToString(logLevel);
    std::string timeStamp = getTimeStamp();
    std::string logMessage = "[" + timeStamp + "][" + logLevelString + "]";
    if (!file.empty())
    {
        if(file.size() > 10)
        {
            file = ".." + file.substr(file.size() - 20);
        }
        logMessage += "[" + file + ":" + std::to_string(line) + "] ";
    }
    logMessage += message;
    return logMessage;
}

std::string Logger::logLevelToString(LOG_LEVEL logLevel)
{
    switch (logLevel)
    {
        case DEBUG:
            return "DEBUG";
        case ERROR:
            return "ERROR";
        case FATAL:
            return "FATAL";
        case INFO:
            return "INFO";
        case WARNING:
            return "WARNING";
        default:
            return "UNKNOWN";
    }
}

}
