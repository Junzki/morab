cmake_minimum_required(VERSION 3.8)


include_directories(${PROJECT_SOURCE_DIR}/src)

find_package(Catch2 REQUIRED)
find_package(LibXml2 REQUIRED)
include_directories(${LibXml2_INCLUDE_DIR})

set(PROJECT_LIBRARY_NAME "lib${PROJECT_NAME}")
add_library(${PROJECT_LIBRARY_NAME} STATIC ${${PROJECT_NAME}_FILES})

set(TEST_LIBRARIES Catch2::Catch2 ${PROJECT_LIBRARY_NAME})
list(APPEND ${TEST_LIBRARIES} ${LIBXML2_LIBRARIES})

add_executable(test_extract "test_extract.cpp")
target_link_libraries(test_extract ${TEST_LIBRARIES})


include(Catch)
catch_discover_tests(test_extract)
