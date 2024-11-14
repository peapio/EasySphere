#include "log.h"
#include "utils.h"

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger::~Logger()
{
    if(m_logFileStream.is_open())
    {
        m_logFileStream.close();
    }
}

int Logger::setLogFile(std::string& filename)
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

void Logger::log(LOG_LEVEL logLevel, const std::string& file, int line, const std::string& format, ...)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (logLevel >= m_logLevel)
    {
        va_list args;
        va_start(args, format);
        char message[1024];
        vsnprintf(message, sizeof(message), format.c_str(), args);
        va_end(args);
        std::string logMessage = formatLog(logLevel, file, line, message);
        m_currentFileSize += logMessage.size();

        if(m_currentFileSize > m_maxFileSize && m_maxFileSize > 0)
        {
            rotateLogFile();
        }

        std::cout << logMessage << std::endl;

        if(m_logFileStream.is_open())
        {
            m_logFileStream << logMessage << std::endl;
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

std::string Logger::formatLog(LOG_LEVEL logLevel, const std::string& file, int line, const std::string& message)
{
    std::string logLevelString = logLevelToString(logLevel);
    std::string timeStamp = getTimeStamp();
    std::string logMessage = "[" + timeStamp + "][" + logLevelString + "]";
    if (!file.empty())
    {
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