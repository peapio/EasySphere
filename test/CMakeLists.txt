

file(GLOB SRC *.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src/deps)

link_directories(${PROJECT_SOURCE_DIR}/lib)

link_libraries(net common)
add_executable(client ${SRC})