
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

add_library(
    tapelib
    FileTape.cpp 
    TapeSorter.cpp 
)

target_include_directories(tapelib PUBLIC include)
