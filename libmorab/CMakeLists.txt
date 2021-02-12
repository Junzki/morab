cmake_minimum_required(VERSION 3.8)

set(LIBMORAB libmorab)


find_package(unofficial-curlpp CONFIG REQUIRED)
# vcpkg may not add installed libraries to include_directories
# add them explicitly.
find_path(VCPKG_INCLUDE_DIR curlpp)
include_directories(${VCPKG_INCLUDE_DIR})
message("-- Added vcpkg include directory: ${VCPKG_INCLUDE_DIR}")

find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

set(TARGET_LIBRARIES unofficial::curlpp::curlpp ${LIBXML2_LIBRARIES})
if(WIN32)
list(APPEND TARGET_LIBRARIES shlwapi)
endif()

file(GLOB_RECURSE ${LIBMORAB}_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(${LIBMORAB} STATIC ${${LIBMORAB}_FILES})
target_link_libraries(${LIBMORAB} ${TARGET_LIBRARIES})
