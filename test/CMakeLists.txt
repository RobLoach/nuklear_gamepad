# nuklear_gamepad_test
add_executable(nuklear_gamepad_test nuklear_gamepad_test.c)

# C99 Standard
set_property(TARGET nuklear_gamepad_test PROPERTY C_STANDARD 99)
set_property(TARGET nuklear_gamepad_test PROPERTY C_STANDARD_REQUIRED TRUE)

# Strict Warnings and Errors
if(MSVC)
    target_compile_options(nuklear_gamepad_test PRIVATE /W4 /WX)
else()
    target_compile_options(nuklear_gamepad_test PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Set up the test
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
add_test(NAME nuklear_gamepad_test COMMAND nuklear_gamepad_test)
