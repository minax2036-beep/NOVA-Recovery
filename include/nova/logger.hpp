#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace nova {

enum class LogLevel { Info, Warning, Error };

struct LogEntry {
    LogLevel level;
    std::string timestamp;
    std::string message;
};

class Logger {
public:
    // Keep only the newest entries to bound memory use on small devices.
    static constexpr std::size_t MaxEntries = 256;

    static void write(LogLevel level, const std::string& message);
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);

    static std::vector<LogEntry> snapshot();
    static std::size_t count();
    static void clear();

private:
    static std::string makeTimestamp();
};

} // namespace nova
