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

# Tests
option(NUKLEAR_GAMEPAD_BUILD_TESTS "Build Tests" ${NUKLEAR_GAMEPAD_IS_MAIN})
if (NUKLEAR_GAMEPAD_BUILD_TESTS)
    include(CTest)
    enable_testing()
    if (BUILD_TESTING)
        set(CTEST_CUSTOM_TESTS_IGNORE
            pkg-config--static
        )
        # Always print verbose output when tests fail if run using `make test`.
        list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure -V --debug")
        add_subdirectory(test)
    endif()
endif()
