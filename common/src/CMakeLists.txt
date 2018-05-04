cmake_minimum_required(VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR}/include)

# common library source files
set(COMMON_SOURCE
    Common.cpp
)

#-------------------------------------------------------------------------------
# Building static library (always)
#-------------------------------------------------------------------------------
add_library(common STATIC ${COMMON_SOURCE})

#-------------------------------------------------------------------------------
# Installation configurations
#-------------------------------------------------------------------------------
install(TARGETS common DESTINATION lib)

# install headers
install(FILES ${COMMON_HEADER} DESTINATION include)