#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <stdarg.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#ifndef LOG_PRINTF_BUFFER_SIZE
#define LOG_PRINTF_BUFFER_SIZE 256
#endif

namespace LoggerNS
{

    class Logger
    {
    public:
        // Renamed enum to avoid conflicts
        enum class LogLevel
        {
            Debug,
            Info,
            Warning,
            Error
        };

        void begin(Print &out, LogLevel level = LogLevel::Info);
        void setLogLevel(LogLevel level);
        bool isLogging(LogLevel levelCheck = LogLevel::Info) const;
        Logger &prefix(const char *file, int line, LogLevel current_level);
        void println();
        char *str();
        static Logger &instance();
        LogLevel level() const;
        void safePrint(const char *fmt, ...);

    protected:
        Logger();

        const char *levelName(LogLevel level) const;
        void printPrefix(const char *file, int line, LogLevel current_level) const;

    private:
        Print *log_print_ptr = nullptr;
        LogLevel log_level = LogLevel::Info;
        char print_buffer[LOG_PRINTF_BUFFER_SIZE] = {0};
        SemaphoreHandle_t mutex = nullptr;
    };

    // Global singleton
    extern Logger &GlobalLogger;

// -------------------- Logging macros --------------------
#define LOG_O(level, fmt, ...)                                    \
    do                                                            \
    {                                                             \
        LoggerNS::GlobalLogger.prefix(__FILE__, __LINE__, level); \
        LoggerNS::GlobalLogger.safePrint(fmt, ##__VA_ARGS__);     \
        LoggerNS::GlobalLogger.println();                         \
    } while (0)

#define LOG_D(fmt, ...) LOG_O(LoggerNS::Logger::LogLevel::Debug, fmt, ##__VA_ARGS__)
#define LOG_I(fmt, ...) LOG_O(LoggerNS::Logger::LogLevel::Info, fmt, ##__VA_ARGS__)
#define LOG_W(fmt, ...) LOG_O(LoggerNS::Logger::LogLevel::Warning, fmt, ##__VA_ARGS__)
#define LOG_E(fmt, ...) LOG_O(LoggerNS::Logger::LogLevel::Error, fmt, ##__VA_ARGS__)

#define TRACE_D() LOG_D("Trace")
#define TRACE_I() LOG_I("Trace")
#define TRACE_W() LOG_W("Trace")
#define TRACE_E() LOG_E("Trace")

} // namespace LoggerNS

#endif // LOGGER_H
