cmake_minimum_required(VERSION 2.8)

set(G3D_SOURCE_LIST
    main.cpp
    Camera.cpp
	SoftwareRenderer.cpp
	SceneObject.cpp
	Scene.cpp
	TriangleMesh.cpp
)

#-------------------------------------------------------------------------------
# Set include directories
#-------------------------------------------------------------------------------
include_directories(
  ${SDL_INCLUDE_DIRS}
  ${COMMON_INCLUDE_DIRS}
  ${TINYOBJ_INCLUDE_DIRS}
)

#-------------------------------------------------------------------------------
# Set link directories
#-------------------------------------------------------------------------------
link_directories(
  ${SDL_LIBRARY_DIRS}
  ${COMMON_LIBRARY_DIRS}
)

#-------------------------------------------------------------------------------
# Add executable
#-------------------------------------------------------------------------------
add_executable(g3d ${G3D_SOURCE_LIST})

target_link_libraries( g3d
    common ${COMMON_LIBRARIES}
    SDL2 ${SDL_LIBRARIES}
)

# Put executable in build directory root
set(EXECUTABLE_OUTPUT_PATH ..)

# Install to project root
install(TARGETS g3d DESTINATION ${G3D_SOURCE_DIR})

# Copy SDL DLLs to the build directory
if(WIN32)
    foreach(SDL_DLL ${SDL_RUNTIMELIBS})
        add_custom_command(TARGET g3d POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SDL_DLL}"
                $<TARGET_FILE_DIR:g3d>)
    endforeach(SDL_DLL)
endif(WIN32)