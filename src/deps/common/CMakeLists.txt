
message(STATUS "Building Common")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SRC *.cpp)

add_library(common STATIC ${SRC})
