#include "Logger.h"
#include <time.h>
#include <stdarg.h>

#ifdef ARDUINO_ARCH_ESP32
#include "HWCDC.h"
#endif

#ifdef ARDUINO_ARCH_ESP32
HWCDC USBSerial; // Definition of the USBSerial object
#define SERIAL USBSerial
#else
#define SERIAL Serial // Fallback to standard Serial for other platforms    
#endif

// 初始化静态实例
Logger* Logger::instance = nullptr;

// 构造函数
Logger::Logger() {
    // 默认设置
    logLevel = LOG_INFO;       // 默认日志级别为INFO
    serialEnabled = true;      // 默认启用串口输出
    fileEnabled = false;       // 默认不启用文件输出
    logFilePath = "/logs.txt"; // 默认日志文件路径
    maxFileSize = 1024 * 10;   // 默认最大文件大小为10KB
    currentFileSize = 0;
    
    // 初始化串口
    SERIAL.begin(115200);
    delay(100);
    
    // 初始化文件系统
    if (fileEnabled) {
        initFileSystem();
        
        // 获取当前日志文件大小
        File logFile;
        if (openLogFile(logFile, "r")) {
            currentFileSize = logFile.size();
            logFile.close();
        }
    }
}

// 获取单例实例
Logger* Logger::getInstance() {
    if (instance == nullptr) {
        instance = new Logger();
    }
    return instance;
}

// 设置日志级别
void Logger::setLogLevel(LogLevel level) {
    logLevel = level;
}

// 启用/禁用串口输出
void Logger::enableSerial(bool enable) {
    serialEnabled = enable;
}

// 启用/禁用文件输出
void Logger::enableFile(bool enable) {
    fileEnabled = enable;
    if (fileEnabled && !LittleFS.begin()) {
        initFileSystem();
    }
}

// 设置日志文件路径
void Logger::setLogFilePath(const String& path) {
    logFilePath = path;
}

// 获取当前日志文件路径
String Logger::getLogFilePath() {
    return logFilePath;
}

// 设置最大日志文件大小
void Logger::setMaxFileSize(unsigned long size) {
    maxFileSize = size;
}

// 获取最大日志文件大小
unsigned long Logger::getMaxFileSize() {
    return maxFileSize;
}

// 获取当前日志文件大小
unsigned long Logger::getCurrentFileSize() {
    return currentFileSize;
}

// 清除日志文件
bool Logger::clearLogFile() {
    if (!fileEnabled || !LittleFS.begin()) {
        return false;
    }
    
    if (LittleFS.exists(logFilePath)) {
        if (LittleFS.remove(logFilePath)) {
            currentFileSize = 0;
            return true;
        }
    }
    return false;
}

// 初始化文件系统
bool Logger::initFileSystem() {
    if (!LittleFS.begin()) {
        SERIAL.println("LOGGER: 文件系统初始化失败！");
        return false;
    }
    return true;
}

// 打开日志文件
bool Logger::openLogFile(File& file, const char* mode) {
    if (!LittleFS.begin()) {
        return false;
    }
    
    if (strcmp(mode, "r") == 0) {
        file = LittleFS.open(logFilePath, "r");
    } else if (strcmp(mode, "w") == 0) {
        file = LittleFS.open(logFilePath, "w");
    } else if (strcmp(mode, "a") == 0) {
        file = LittleFS.open(logFilePath, "a");
    } else {
        return false;
    }
    
    return file;
}

// 日志文件轮转
void Logger::rotateLogFileIfNeeded() {
    if (!fileEnabled) return;
    
    if (currentFileSize >= maxFileSize) {
        // 创建临时文件
        String tempFilePath = logFilePath + ".temp";
        File oldFile;
        File newFile;
        
        if (openLogFile(oldFile, "r") && LittleFS.open(tempFilePath, "w")) {
            // 计算要保留的大小（保留后半部分的日志）
            unsigned long keepSize = maxFileSize / 2;
            unsigned long skipSize = currentFileSize - keepSize;
            
            // 跳过前半部分
            oldFile.seek(skipSize);
            
            // 复制后半部分到临时文件
            newFile = LittleFS.open(tempFilePath, "w");
            if (newFile) {
                while (oldFile.available()) {
                    newFile.write(oldFile.read());
                }
                newFile.close();
            }
            oldFile.close();
            
            // 删除旧文件
            LittleFS.remove(logFilePath);
            
            // 重命名临时文件
            LittleFS.rename(tempFilePath, logFilePath);
            
            // 更新文件大小
            File file = LittleFS.open(logFilePath, "r");
            if (file) {
                currentFileSize = file.size();
                file.close();
            } else {
                currentFileSize = 0;
            }
        }
    }
}

// 获取日志级别的字符串表示
String Logger::getLevelString(LogLevel level) {
    switch (level) {
        case LOG_ERROR: return "E";
        case LOG_WARN:  return "W";
        case LOG_INFO:  return "I";
        case LOG_DEBUG: return "D";
        default:        return "?";
    }
}

// 实际写入日志的方法
void Logger::writeLog(LogLevel level, const String& tag, const String& message) {
    // 检查日志级别
    if (level > logLevel) {
        return;
    }
    
    // 获取当前时间（如果有RTC可以获取真实时间）
    unsigned long now = millis();
    unsigned long seconds = now / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    
    seconds %= 60;
    minutes %= 60;
    
    // 格式化时间戳
    char timestamp[16];
    sprintf(timestamp, "%02lu:%02lu:%02lu", hours, minutes, seconds);
    
    // 构建日志消息
    String levelStr = getLevelString(level);
    String logMessage = String(timestamp) + " " + levelStr + "-[" + tag + "]: " + message;
    
    // 写入串口
    if (serialEnabled) {
        SERIAL.println(logMessage);
    }
    
    // 写入文件
    if (fileEnabled) {
        // 检查是否需要轮转日志文件
        rotateLogFileIfNeeded();
        
        // 打开日志文件并追加内容
        File logFile;
        if (openLogFile(logFile, "a")) {
            // 写入日志内容
            size_t bytesWritten = logFile.println(logMessage);
            currentFileSize += bytesWritten;
            logFile.close();
        }
    }
}

// 基本日志输出方法
void Logger::error(const String& tag, const String& message) {
    writeLog(LOG_ERROR, tag, message);
}

void Logger::warn(const String& tag, const String& message) {
    writeLog(LOG_WARN, tag, message);
}

void Logger::info(const String& tag, const String& message) {
    writeLog(LOG_INFO, tag, message);
}

void Logger::debug(const String& tag, const String& message) {
    writeLog(LOG_DEBUG, tag, message);
}

// 格式化日志输出方法
void Logger::error(const String& tag, const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    writeLog(LOG_ERROR, tag, buffer);
}

void Logger::warn(const String& tag, const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    writeLog(LOG_WARN, tag, buffer);
}

void Logger::info(const String& tag, const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    writeLog(LOG_INFO, tag, buffer);
}

void Logger::debug(const String& tag, const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    writeLog(LOG_DEBUG, tag, buffer);
} 