cmake_minimum_required(VERSION 2.8)
project(G3D)

# Set include paths
set(G3D_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include")
#-------------------------------------------------------------------------------
# Add subdirectories
#-------------------------------------------------------------------------------
add_subdirectory(common)
add_subdirectory(src)

# Install settings
set(CMAKE_INSTALL_PREFIX "${G3D_SOURCE_DIR}/")
