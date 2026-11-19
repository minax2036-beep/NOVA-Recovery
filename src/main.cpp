#include "nova/logger.hpp"

int main() {
    nova::Logger::write(nova::LogLevel::Info, "NOVA Core starting");
    nova::Logger::write(nova::LogLevel::Info, "Logger initialized");
    nova::Logger::write(nova::LogLevel::Warn, "Device backend is not connected yet");
    nova::Logger::write(nova::LogLevel::Info, "Prototype finished");
    return 0;
}
