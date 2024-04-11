cmake_minimum_required(VERSION 3.11)
project(nuklear_gamepad
    DESCRIPTION "nuklear_gamepad: Gamepad API for Nuklear"
    HOMEPAGE_URL "https://github.com/robloach/nuklear_gamepad"
    VERSION 0.1.0
    LANGUAGES C
)

# nuklear_gamepad
add_library(nuklear_gamepad INTERFACE)
target_include_directories(nuklear_gamepad INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Options
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(NUKLEAR_GAMEPAD_IS_MAIN TRUE)
else()
    set(NUKLEAR_GAMEPAD_IS_MAIN FALSE)
endif()
