

file(GLOB SRC *.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src/deps)

link_directories(${PROJECT_SOURCE_DIR}/lib)
link_libraries(common)
add_executable(server ${SRC})