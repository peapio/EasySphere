
message(STATUS "Building Net")

include_directories(${PROJECT_SOURCE_DIR}/src/deps)
file(GLOB SRC *.cpp)

link_directories(${PROJECT_SOURCE_DIR}/lib)

link_libraries(common)
add_library(net STATIC ${SRC})
