#include "nova/logger.hpp"

#include <iostream>

int main() {
    nova::Logger::info("NOVA Core starting");
    nova::Logger::info("Build: 0.0.2-dev");
    nova::Logger::info("Logger initialized (memory ring: 256 entries)");
    nova::Logger::warning("Device display backend is not implemented");
    nova::Logger::error("Demo error entry");

    std::cout << "\n--- Log snapshot (" << nova::Logger::count() << " entries) ---\n";
    for (const auto& entry : nova::Logger::snapshot()) {
        const char* level = entry.level == nova::LogLevel::Info ? "INFO" :
                            entry.level == nova::LogLevel::Warning ? "WARN" : "ERROR";
        std::cout << '[' << entry.timestamp << "] [" << level << "] "
                  << entry.message << '\n';
    }

    return 0;
}
