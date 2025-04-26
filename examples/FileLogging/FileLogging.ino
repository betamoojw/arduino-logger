/**
 * Logger 库文件日志示例
 * 
 * 此示例展示了Logger库的文件日志功能，包括：
 * - 启用文件日志
 * - 设置日志文件路径
 * - 设置最大日志文件大小
 * - 日志文件轮转功能
 */

#include <Arduino.h>
#include <Logger.h>

// 模拟数据
int loopCount = 0;
unsigned long startTime = 0;

void setup() {
  // 初始化串口
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n===== Logger库文件日志示例 =====\n");
  
  // 获取日志实例
  Logger* logger = Logger::getInstance();
  
  // 设置日志级别
  logger->setLogLevel(LOG_DEBUG);
  
  // 设置日志文件路径（默认为/logs.txt）
  logger->setLogFilePath("/system_logs.txt");
  
  // 设置最大日志文件大小（这里设置较小的值以便快速观察日志轮转效果）
  // 默认为10KB，这里设置为2KB便于测试
  logger->setMaxFileSize(2 * 1024);
  
  // 启用文件日志功能
  logger->enableFile(true);
  
  // 清除以前的日志文件（可选）
  if (logger->clearLogFile()) {
    Serial.println("旧日志文件已清除");
  } else {
    Serial.println("清除日志文件失败或文件不存在");
  }
  
  logger->info("SETUP", "文件日志功能已启用");
  logger->info("SETUP", "日志文件: %s", logger->getLogFilePath().c_str());
  logger->info("SETUP", "最大文件大小: %lu 字节", logger->getMaxFileSize());
  
  // 记录启动时间
  startTime = millis();
  logger->info("SETUP", "系统启动时间: %lu ms", startTime);
}

void loop() {
  loopCount++;
  
  // 记录循环计数日志
  LOG_D("LOOP", "循环次数: %d", loopCount);
  
  // 每10次循环记录一次系统运行时间
  if (loopCount % 10 == 0) {
    unsigned long uptime = millis() - startTime;
    LOG_I("SYSTEM", "系统运行时间: %lu ms, 文件大小: %lu 字节", 
           uptime, LOG->getCurrentFileSize());
  }
  
  // 模拟产生大量日志以触发日志轮转
  if (loopCount % 20 == 0) {
    LOG_W("TEST", "产生大量日志以测试日志轮转...");
    
    // 生成多条日志记录
    for (int i = 0; i < 30; i++) {
      LOG_I("TEST", "测试日志轮转 - 记录 #%d - 随机数据: %d", 
             i, random(1000, 9999));
    }
    
    LOG_I("TEST", "日志轮转测试完成，当前文件大小: %lu 字节", 
           LOG->getCurrentFileSize());
  }
  
  delay(500);
} 