# libretro-common
include(FetchContent)
FetchContent_Declare(
    libretrocommon
    GIT_REPOSITORY https://github.com/libretro/libretro-common.git
    GIT_TAG b0c348ea5543c4d7fb0bc479258aa6988b20c0c9
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(libretrocommon)
#include_directories(${libretrocommon_SOURCE_DIR})
