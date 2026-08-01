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

# nuklear_gamepad_test_prefix: Long NK_GAMEPAD_NAME_PREFIX
add_executable(nuklear_gamepad_test_prefix nuklear_gamepad_test_prefix.c)

# C99 Standard
set_property(TARGET nuklear_gamepad_test_prefix PROPERTY C_STANDARD 90)
set_property(TARGET nuklear_gamepad_test_prefix PROPERTY C_STANDARD_REQUIRED TRUE)

# Strict Warnings and Errors
if(MSVC)
    target_compile_options(nuklear_gamepad_test_prefix PRIVATE /W4 /WX /UNDEBUG)
else()
    target_compile_options(nuklear_gamepad_test_prefix PRIVATE -Wall -Wextra -Wpedantic -UNDEBUG)
endif()

# Set up the test
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
set(CTEST_OUTPUT_ON_FAILURE TRUE)
add_test(NAME nuklear_gamepad_test COMMAND nuklear_gamepad_test)
add_test(NAME nuklear_gamepad_test_prefix COMMAND nuklear_gamepad_test_prefix)
