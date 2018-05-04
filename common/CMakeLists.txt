cmake_minimum_required(VERSION 2.8)
project(common)

set(COMMON_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-------------------------------------------------------------------------------
# CMake modules
#-------------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH ${COMMON_SOURCE_DIR}/cmake/modules/)

#-------------------------------------------------------------------------------
# common paths
#-------------------------------------------------------------------------------
set(COMMON_INCLUDE_DIRS ${COMMON_SOURCE_DIR}/include CACHE PATH "" FORCE)


#-------------------------------------------------------------------------------
# Add subdirectories
#-------------------------------------------------------------------------------
add_subdirectory(${COMMON_SOURCE_DIR}/deps/SDL2-2.0.8)

#-------------------------------------------------------------------------------
# Set include directories
#-------------------------------------------------------------------------------
include_directories(
  ${COMMON_INCLUDE_DIRS}
)

#-------------------------------------------------------------------------------
# Add subdirectories
#-------------------------------------------------------------------------------
add_subdirectory(src)

# Install settings
set(CMAKE_INSTALL_PREFIX "${COMMON_SOURCE_DIR}")
