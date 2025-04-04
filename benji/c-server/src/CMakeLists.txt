include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(server server.c http_helpers.c)
