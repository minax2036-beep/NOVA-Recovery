#pragma once

#include <string>

namespace nova {

enum class LogLevel { Info, Warn, Error };

class Logger {
public:
    static void write(LogLevel level, const std::string& message);
};

} // namespace nova
