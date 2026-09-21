# nuklear_gamepad_test
add_executable(nuklear_gamepad_test nuklear_gamepad_test.c)

# C99 Standard
set_property(TARGET nuklear_gamepad_test PROPERTY C_STANDARD 90)
set_property(TARGET nuklear_gamepad_test PROPERTY C_STANDARD_REQUIRED TRUE)

# Strict Warnings and Errors
if(MSVC)
    target_compile_options(nuklear_gamepad_test PRIVATE /W4 /WX /UNDEBUG)
else()
    target_compile_options(nuklear_gamepad_test PRIVATE -Wall -Wextra -Wpedantic -UNDEBUG)
endif()

# nuklear_gamepad_sdl3_test
add_executable(nuklear_gamepad_sdl3_test nuklear_gamepad_sdl3_test.c)

# Resolves #include <SDL3/SDL.h> to the headless SDL3 test stub.
target_include_directories(nuklear_gamepad_sdl3_test PRIVATE stubs)

# C90 Standard
set_property(TARGET nuklear_gamepad_sdl3_test PROPERTY C_STANDARD 90)
set_property(TARGET nuklear_gamepad_sdl3_test PROPERTY C_STANDARD_REQUIRED TRUE)

# Strict Warnings and Errors
if(MSVC)
    target_compile_options(nuklear_gamepad_sdl3_test PRIVATE /W4 /WX /UNDEBUG)
else()
    target_compile_options(nuklear_gamepad_sdl3_test PRIVATE -Wall -Wextra -Wpedantic -UNDEBUG)
endif()

# Set up the tests
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
set(CTEST_OUTPUT_ON_FAILURE TRUE)
add_test(NAME nuklear_gamepad_test COMMAND nuklear_gamepad_test)
add_test(NAME nuklear_gamepad_sdl3_test COMMAND nuklear_gamepad_sdl3_test)
