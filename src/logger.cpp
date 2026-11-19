#include "nova/logger.hpp"

#include <chrono>
#include <ctime>
#include <iostream>
#include <mutex>

namespace nova {
namespace {
std::mutex logMutex;
std::vector<LogEntry> entries;

const char* levelName(LogLevel level) {
    switch (level) {
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARN";
        case LogLevel::Error: return "ERROR";
    }
    return "UNKNOWN";
}
} // namespace

std::string Logger::makeTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif
    char buffer[16]{};
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", &tm);
    return buffer;
}

void Logger::write(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    LogEntry entry{level, makeTimestamp(), message};

    if (entries.size() >= MaxEntries) {
        entries.erase(entries.begin());
    }
    entries.push_back(entry);

    std::ostream& stream = (level == LogLevel::Error) ? std::cerr : std::cout;
    stream << '[' << entry.timestamp << "] [" << levelName(level) << "] "
           << entry.message << '\n';
}

void Logger::info(const std::string& message) { write(LogLevel::Info, message); }
void Logger::warning(const std::string& message) { write(LogLevel::Warning, message); }
void Logger::error(const std::string& message) { write(LogLevel::Error, message); }

std::vector<LogEntry> Logger::snapshot() {
    std::lock_guard<std::mutex> lock(logMutex);
    return entries;
}

std::size_t Logger::count() {
    std::lock_guard<std::mutex> lock(logMutex);
    return entries.size();
}

void Logger::clear() {
    std::lock_guard<std::mutex> lock(logMutex);
    entries.clear();
}

} // namespace nova
