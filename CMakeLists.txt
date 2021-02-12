# CMakeList.txt : CMake project for Project Takeout, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("morab" CXX)
set(CMAKE_CXX_STANDARD 14)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_subdirectory(libmorab)
set(TARGET_LIBRARIES libmorab)


# Cli
set(MORAB_CLI "morab")
file(GLOB_RECURSE ${MORAB_CLI}_FILES "${PROJECT_SOURCE_DIR}/cli/*.cpp")
add_executable(${MORAB_CLI} ${${MORAB_CLI}_FILES})
target_link_libraries(${MORAB_CLI} PRIVATE ${TARGET_LIBRARIES})

# Server
set(MORAB_SERVER "morab-server")
file(GLOB_RECURSE ${MORAB_SERVER}_FILES "${PROJECT_SOURCE_DIR}/server/*.cpp")
add_executable(${MORAB_SERVER} ${${MORAB_SERVER}_FILES})
target_link_libraries(${MORAB_SERVER} PRIVATE ${TARGET_LIBRARIES})

# Tests
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    include(CTest)

    if (${BUILD_TESTING})
        add_subdirectory(tests)
    endif()
endif()
