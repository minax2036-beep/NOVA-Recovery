cmake_minimum_required(VERSION 3.16)
project(NOVARecovery VERSION 0.0.5 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(nova-core
    src/main.cpp
    src/logger.cpp
    src/renderer.cpp
    src/framebuffer.cpp
    src/recovery_display_stub.cpp
    src/recovery_ui.cpp
)
target_include_directories(nova-core PRIVATE include)

# Host-only startup visualization and simulation (not a device boot image).
add_executable(nova-recovery-boot-sim
    src/recovery_main.cpp
    src/renderer.cpp
)
target_include_directories(nova-recovery-boot-sim PRIVATE include)

add_executable(nova-recovery-ui-test
    tests/recovery_ui_test.cpp
    src/recovery_ui.cpp
    src/logger.cpp
    src/renderer.cpp
)
target_include_directories(nova-recovery-ui-test PRIVATE include)

add_executable(nova-minui-adapter-test
    tests/minui_adapter_test.cpp
    tests/minui_stub.cpp
    src/renderer.cpp
    src/recovery_display_minui.cpp
)
target_include_directories(nova-minui-adapter-test PRIVATE include tests/minui_stub)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    foreach(target nova-core nova-recovery-boot-sim nova-minui-adapter-test nova-recovery-ui-test)
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endforeach()
endif()

enable_testing()
add_test(NAME nova-core-smoke COMMAND nova-core)
add_test(NAME nova-boot-simulation COMMAND nova-recovery-boot-sim)
add_test(NAME nova-boot-display-error COMMAND nova-recovery-boot-sim --fail-display)
set_tests_properties(nova-boot-display-error PROPERTIES WILL_FAIL TRUE)
add_test(NAME nova-boot-ui-error COMMAND nova-recovery-boot-sim --fail-ui)
set_tests_properties(nova-boot-ui-error PROPERTIES WILL_FAIL TRUE)
add_test(NAME nova-minui-adapter COMMAND nova-minui-adapter-test)
add_test(NAME nova-recovery-ui COMMAND nova-recovery-ui-test)
