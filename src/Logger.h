#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <LittleFS.h>

// 定义日志级别
enum LogLevel {
    LOG_ERROR = 0,
    LOG_WARN = 1,
    LOG_INFO = 2,
    LOG_DEBUG = 3
};

class Logger {
private:
    static Logger* instance;
    
    // 日志设置
    LogLevel logLevel;             // 当前日志级别
    bool serialEnabled;            // 是否输出到串口
    bool fileEnabled;              // 是否输出到文件
    String logFilePath;            // 日志文件路径
    unsigned long maxFileSize;     // 最大日志文件大小 (字节)
    unsigned long currentFileSize; // 当前日志文件大小
    
    // 私有构造函数（单例模式）
    Logger();
    
    // 实际写入日志的方法
    void writeLog(LogLevel level, const String& tag, const String& message);
    
    // 获取日志级别的字符串表示
    String getLevelString(LogLevel level);
    
    // 文件系统操作
    bool initFileSystem();
    bool openLogFile(File& file, const char* mode);
    void rotateLogFileIfNeeded();
    
public:
    // 获取单例实例
    static Logger* getInstance();
    
    // 设置日志级别
    void setLogLevel(LogLevel level);
    
    // 启用/禁用串口输出
    void enableSerial(bool enable);
    
    // 启用/禁用文件输出
    void enableFile(bool enable);
    
    // 设置日志文件路径
    void setLogFilePath(const String& path);
    
    // 获取当前日志文件路径
    String getLogFilePath();
    
    // 设置最大日志文件大小
    void setMaxFileSize(unsigned long size);
    
    // 获取最大日志文件大小
    unsigned long getMaxFileSize();
    
    // 获取当前日志文件大小
    unsigned long getCurrentFileSize();
    
    // 清除日志文件
    bool clearLogFile();
    
    // 日志输出方法
    void error(const String& tag, const String& message);
    void warn(const String& tag, const String& message);
    void info(const String& tag, const String& message);
    void debug(const String& tag, const String& message);
    
    // 带格式化的日志输出方法
    void error(const String& tag, const char* format, ...);
    void warn(const String& tag, const char* format, ...);
    void info(const String& tag, const char* format, ...);
    void debug(const String& tag, const char* format, ...);
};

// 简化使用的宏
#define LOG Logger::getInstance()
#define LOG_E(tag, ...) Logger::getInstance()->error(tag, __VA_ARGS__)
#define LOG_W(tag, ...) Logger::getInstance()->warn(tag, __VA_ARGS__)
#define LOG_I(tag, ...) Logger::getInstance()->info(tag, __VA_ARGS__)
#define LOG_D(tag, ...) Logger::getInstance()->debug(tag, __VA_ARGS__)

#endif // LOGGER_H 