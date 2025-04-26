# Logger

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform: ESP8266](https://img.shields.io/badge/Platform-ESP8266-blue.svg)](https://github.com/esp8266/Arduino)
[![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-blue.svg)](https://github.com/espressif/arduino-esp32)

一个轻量级的ESP8266和ESP32日志库，提供格式化的日志输出、多种日志级别和多种输出目标（串口和文件）。

## 特性

- 支持多种日志级别：ERROR, WARN, INFO, DEBUG
- 支持日志输出到串口（默认启用）
- 支持日志输出到文件系统（默认禁用）
- 自动限制日志文件大小和循环写入，避免占满存储空间
- 提供便捷的宏定义简化日志输出
- 支持格式化输出
- 单例模式设计，全局方便访问

## 安装

### PlatformIO

```
pio lib install "Logger"
```

### 手动安装

1. 下载此库的最新版本
2. 解压缩并将文件夹重命名为"Logger"
3. 将文件夹移动到Arduino/libraries目录或PlatformIO项目的lib目录下
4. 重启Arduino IDE或PlatformIO IDE

## 使用方法

### 基本用法

```cpp
#include <Arduino.h>
#include <Logger.h>

void setup() {
  Serial.begin(115200);
  
  // 获取日志实例
  Logger* logger = Logger::getInstance();
  
  // 设置日志级别（默认为INFO）
  logger->setLogLevel(LOG_DEBUG);
  
  // 输出日志
  logger->error("TAG", "这是一条错误日志");
  logger->warn("TAG", "这是一条警告日志");
  logger->info("TAG", "这是一条信息日志");
  logger->debug("TAG", "这是一条调试日志");
  
  // 格式化输出
  logger->info("TAG", "当前温度: %.1f°C", 25.5);
}

void loop() {
  // 使用宏输出日志
  LOG_E("TAG", "错误：传感器未连接");
  LOG_W("TAG", "警告：电池电量低");
  LOG_I("TAG", "信息：当前温度: %.1f°C", 25.5);
  LOG_D("TAG", "调试：循环执行中");
  
  delay(1000);
}
```

### 启用文件日志

```cpp
void setup() {
  Serial.begin(115200);
  
  // 获取日志实例
  Logger* logger = Logger::getInstance();
  
  // 设置日志文件路径（默认为/logs.txt）
  logger->setLogFilePath("/my_logs.txt");
  
  // 设置最大日志文件大小（默认为10KB）
  logger->setMaxFileSize(1024 * 20); // 设置为20KB
  
  // 启用文件日志
  logger->enableFile(true);
  
  // 清除旧的日志文件（可选）
  logger->clearLogFile();
}
```

## 日志级别

日志库支持以下日志级别（从低到高）：

1. **ERROR** (LOG_ERROR): 错误信息，表示发生了严重问题
2. **WARN** (LOG_WARN): 警告信息，表示潜在的问题
3. **INFO** (LOG_INFO): 一般信息，默认级别
4. **DEBUG** (LOG_DEBUG): 调试信息，用于开发调试

日志库只会输出级别小于或等于当前设置级别的日志。例如，如果设置级别为INFO，则会输出ERROR、WARN和INFO日志，但不会输出DEBUG日志。

## API参考

### 主要类

`Logger` - 日志管理类，使用单例模式设计

### 日志级别

```cpp
enum LogLevel {
    LOG_ERROR = 0,
    LOG_WARN = 1,
    LOG_INFO = 2,
    LOG_DEBUG = 3
};
```

### 方法

- `static Logger* getInstance()` - 获取Logger单例实例
- `void setLogLevel(LogLevel level)` - 设置日志级别
- `void enableSerial(bool enable)` - 启用/禁用串口输出
- `void enableFile(bool enable)` - 启用/禁用文件输出
- `void setLogFilePath(const String& path)` - 设置日志文件路径
- `String getLogFilePath()` - 获取当前日志文件路径
- `void setMaxFileSize(unsigned long size)` - 设置最大日志文件大小
- `unsigned long getMaxFileSize()` - 获取最大日志文件大小
- `unsigned long getCurrentFileSize()` - 获取当前日志文件大小
- `bool clearLogFile()` - 清除日志文件

### 日志输出方法

- `void error(const String& tag, const String& message)` - 输出错误日志
- `void warn(const String& tag, const String& message)` - 输出警告日志
- `void info(const String& tag, const String& message)` - 输出信息日志
- `void debug(const String& tag, const String& message)` - 输出调试日志

### 格式化日志输出方法

- `void error(const String& tag, const char* format, ...)` - 输出格式化错误日志
- `void warn(const String& tag, const char* format, ...)` - 输出格式化警告日志
- `void info(const String& tag, const char* format, ...)` - 输出格式化信息日志
- `void debug(const String& tag, const char* format, ...)` - 输出格式化调试日志

### 便捷宏

- `LOG` - Logger单例实例
- `LOG_E(tag, ...)` - 错误日志宏
- `LOG_W(tag, ...)` - 警告日志宏
- `LOG_I(tag, ...)` - 信息日志宏
- `LOG_D(tag, ...)` - 调试日志宏

## 注意事项

- 文件日志功能依赖于LittleFS文件系统，确保你的项目已经正确配置了文件系统
- 日志文件轮转会在文件达到最大大小时自动执行，会保留后半部分的日志内容
- 默认日志文件大小限制为10KB，可以根据项目需要调整

## 许可证

本库使用MIT许可证。详情请参阅[LICENSE](LICENSE)文件。

## 作者

- **Leo** - *初始工作* - [leoyfm@gmail.com](mailto:leoyfm@gmail.com)

## 贡献

欢迎提交问题和请求。如果您想贡献代码，请提交PR。 