cmake_minimum_required(VERSION 3.20)

file(GLOB TEST_SOURCES "*.cpp")

foreach(TEST_SRC ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} PRIVATE the_stations_core)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
