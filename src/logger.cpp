#include "nova/logger.hpp"

#include <iostream>

namespace nova {

void Logger::write(LogLevel level, const std::string& message) {
    const char* label = "INFO";
    switch (level) {
        case LogLevel::Info: label = "INFO"; break;
        case LogLevel::Warn: label = "WARN"; break;
        case LogLevel::Error: label = "ERROR"; break;
    }
    std::cout << "[" << label << "] " << message << '\n';
}

} // namespace nova
