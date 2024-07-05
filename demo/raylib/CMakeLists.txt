cmake_minimum_required(VERSION 3.11)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
        GIT_SHALLOW 1
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# raylib_nuklear
find_package(raylib_nuklear QUIET)
if (NOT raylib_nuklear_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib_nuklear
        GIT_REPOSITORY https://github.com/RobLoach/raylib-nuklear.git
        GIT_TAG 35995099100620b7e5b14f0fcb5521a6215a2971
    )
    FetchContent_GetProperties(raylib_nuklear)
    if (NOT raylib_nuklear_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib_nuklear)
        add_subdirectory(${raylib_nuklear_SOURCE_DIR} ${raylib_nuklear_BINARY_DIR})
    endif()
endif()

# nuklear_gamepad_demo_raylib
add_executable(nuklear_gamepad_demo_raylib
    main.c
)
target_link_libraries(nuklear_gamepad_demo_raylib PUBLIC
    raylib
    raylib_nuklear
)

# Target C99
set_property(TARGET nuklear_gamepad_demo_raylib PROPERTY C_STANDARD 99)

# Enable warnings
if(MSVC)
  target_compile_options(nuklear_gamepad_demo_raylib PRIVATE /W4 /WX)
else()
  target_compile_options(nuklear_gamepad_demo_raylib PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
