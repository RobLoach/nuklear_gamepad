# libretro-deps
include(FetchContent)
FetchContent_Declare(
    libretrodeps
    GIT_REPOSITORY https://github.com/libretro/libretro-deps.git
    GIT_TAG abf5246b016569759e7d1b0ea91bb98c2e34d160
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(libretrodeps)
#include_directories(${libretrocommon_SOURCE_DIR})
