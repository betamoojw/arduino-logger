/**
 * Logger 库基本使用示例
 * 
 * 此示例展示了Logger库的基本用法，包括：
 * - 不同级别的日志记录
 * - 格式化输出
 * - 使用宏简化日志记录
 */

#include <Arduino.h>
#include <Logger.h>

// 临时模拟传感器数据
float temperature = 25.0;
float humidity = 60.0;
int counter = 0;

void setup() {
  // 初始化串口
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n===== Logger库基本使用示例 =====\n");
  
  // 获取日志实例
  Logger* logger = Logger::getInstance();
  
  // 设置日志级别（默认为INFO）
  // 可选值: LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG
  logger->setLogLevel(LOG_DEBUG);
  
  // 基本日志输出
  logger->error("SETUP", "这是一条错误日志");
  logger->warn("SETUP", "这是一条警告日志");
  logger->info("SETUP", "这是一条信息日志");
  logger->debug("SETUP", "这是一条调试日志");
  
  // 格式化日志输出
  logger->info("SENSOR", "当前温度: %.1f°C, 湿度: %.1f%%", temperature, humidity);
  logger->debug("MEMORY", "可用内存: %d 字节", ESP.getFreeHeap());
  
  // 完成初始化
  logger->info("SETUP", "系统初始化完成");
}

void loop() {
  // 模拟传感器读数变化
  temperature += random(-10, 10) / 10.0;
  humidity += random(-5, 5) / 10.0;
  counter++;
  
  // 使用宏记录日志
  LOG_D("LOOP", "循环计数: %d", counter);
  LOG_I("SENSOR", "温度: %.1f°C, 湿度: %.1f%%", temperature, humidity);
  
  // 模拟错误情况
  if (counter % 10 == 0) {
    LOG_W("SYSTEM", "循环计数为10的倍数: %d", counter);
  }
  
  if (temperature > 30.0) {
    LOG_E("SENSOR", "温度过高警告: %.1f°C", temperature);
    temperature = 25.0; // 重置温度
  }
  
  delay(1000);
} 