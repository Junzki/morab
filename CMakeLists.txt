# CMakeList.txt : CMake project for Project Takeout, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("morab" CXX)
set(CMAKE_CXX_STANDARD 14)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(${PROJECT_NAME}_SOURCE "${PROJECT_SOURCE_DIR}/src")
file(GLOB_RECURSE ${PROJECT_NAME}_FILES "${${PROJECT_NAME}_SOURCE}/*.cpp")

find_package(unofficial-curlpp CONFIG REQUIRED)
find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

set(TARGET_LIBRARIES unofficial::curlpp::curlpp ${LIBXML2_LIBRARIES})
if(WIN32)
list(APPEND TARGET_LIBRARIES shlwapi)
endif()

# Targets
set(TARGET_BINARY "morab")

add_executable(${TARGET_BINARY} ${${PROJECT_NAME}_FILES})
target_link_libraries(${TARGET_BINARY} ${TARGET_LIBRARIES})

# Tests
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    include(CTest)

    if (${BUILD_TESTING})
        add_subdirectory(tests)
    endif()
endif()
