cmake_minimum_required(VERSION 3.11)
project(nuklear_gamepad_pntr
    DESCRIPTION "nuklear_gamepad_pntr"
    HOMEPAGE_URL "https://github.com/robloach/nuklear_gamepad"
    VERSION 0.0.1
    LANGUAGES C
)

# CMAKE Modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/.cmake")

# Targets
option(SDL "Build SDL" TRUE)
option(RAYLIB "Build raylib" TRUE)
option(LIBRETRO "Build libretro" FALSE)
option(CLI "Build CLI" FALSE)
option(WEB "Build Web" FALSE)

# Sources
set(SOURCES
    main.c
)

# Libraries
find_package(pntr_nuklear REQUIRED)
list(APPEND LIBRARIES pntr_nuklear)

# Have pntr_app_starter build the project
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/.cmake")
find_package(pntr_app_starter)
