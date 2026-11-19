cmake_minimum_required(VERSION 3.16)
project(NOVARecovery VERSION 0.0.4 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(nova-core
    src/main.cpp
    src/logger.cpp
    src/renderer.cpp
    src/framebuffer.cpp
    src/recovery_display_stub.cpp
)
target_include_directories(nova-core PRIVATE include)

add_executable(nova-minui-adapter-test
    tests/minui_adapter_test.cpp
    tests/minui_stub.cpp
    src/renderer.cpp
    src/recovery_display_minui.cpp
)
target_include_directories(nova-minui-adapter-test PRIVATE include tests/minui_stub)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(nova-core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(nova-minui-adapter-test PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_test(NAME nova-core-smoke COMMAND nova-core)
add_test(NAME nova-minui-adapter COMMAND nova-minui-adapter-test)
