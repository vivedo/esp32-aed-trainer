#include "Logger.h"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <stdarg.h>
#include <string.h>

Logger::Logger() {
}

void Logger::begin(Print &out, LogLevel level) {
    log_print_ptr = &out;
    log_level = level;
    if (!mutex) {
        mutex = xSemaphoreCreateMutex();
    }
}

void Logger::setLogLevel(LogLevel level) {
    log_level = level;
}

bool Logger::isLogging(LogLevel levelCheck) const {
    return log_print_ptr != nullptr && static_cast<int>(levelCheck) >= static_cast<int>(log_level);
}

Logger &Logger::prefix(const char *file, int line, LogLevel current_level) {
    printPrefix(file, line, current_level);
    return *this;
}

void Logger::println() {
    if (log_print_ptr) {
        log_print_ptr->println(print_buffer);
        log_print_ptr->flush();
    }
    print_buffer[0] = 0;
}

char *Logger::str() {
    return print_buffer;
}

Logger &Logger::instance() {
    static Logger *ptr = nullptr;
    if (!ptr) {
        ptr = new Logger;
        ptr->begin(Serial, LogLevel::Info);
    }
    return *ptr;
}

Logger::LogLevel Logger::level() const {
    return log_level;
}

void Logger::safePrint(const char *fmt, ...) {
    if (!mutex) return;

    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        va_list args;
        va_start(args, fmt);
        vsnprintf(print_buffer, LOG_PRINTF_BUFFER_SIZE, fmt, args);
        va_end(args);
        xSemaphoreGive(mutex);
    }
}

const char *Logger::levelName(LogLevel level) const {
    switch (level) {
        case LogLevel::Debug:
            return "D";
        case LogLevel::Info:
            return "I";
        case LogLevel::Warning:
            return "W";
        case LogLevel::Error:
            return "E";
    }
    return "";
}

void Logger::printPrefix(const char *file, int line, LogLevel current_level) const {
    if (!log_print_ptr) return;

    const char *file_name = strrchr(file, '/') ? strrchr(file, '/') + 1 : file;
    const char *level_code = levelName(current_level);

    log_print_ptr->print(level_code);
    log_print_ptr->print(" ");
    log_print_ptr->print(file_name);
    log_print_ptr->print(" : ");
    log_print_ptr->print(line);
    log_print_ptr->print(" - ");
}

// Define the global singleton
Logger &GlobalLogger = Logger::instance();