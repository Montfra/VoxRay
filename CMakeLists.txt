cmake_minimum_required(VERSION 3.13.1)

Message("")
Message( STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR} )
Message( STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR} )
Message( STATUS "CMAKE_COMMAND: " ${CMAKE_COMMAND} )
Message( STATUS "CMAKE_VERSION: " ${CMAKE_VERSION} )
Message( STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER} )
Message( STATUS "CMAKE_PROJECT_NAME: " ${CMAKE_PROJECT_NAME} )
Message( STATUS "CMAKE_FINDCMAKE: " ${CMAKE_SOURCE_DIR}/CMAKE )

project(VoxRay)

# ------- Set Vars ------- #

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Multi-config generators (Visual Studio, Xcode) otherwise append a per-config
# subfolder (e.g. bin/Release), which breaks the "../ressources" relative
# paths the game uses to load its bitmaps.
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
endforeach()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(SOURCE_FILES    src/main.cpp src/core/Core.h src/game_interfaces/hud.h src/game_interfaces/hud.cpp src/core/Image.cpp src/core/image.h src/game_interfaces/Menu.h src/game_interfaces/Menu.cpp)

Message("")
Message( STATUS "SOURCE entry point : " ${SOURCE_FILES} )
Message("")

# ------- End Set Vars --- #

# ------- Executable ----- #

ADD_EXECUTABLE(VoxRay ${SOURCE_FILES})

# ------- End Executable - #

# ------- Finds ---------- #

FIND_PACKAGE(SDL2 REQUIRED)
Message("")
Message( STATUS "FINDING SDL2" )
IF (${SDL2_FOUND})
    Message( STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    Message( STATUS "SDL2_INCLUDE_DIR:" ${SDL2_INCLUDE_DIR})
    Message( STATUS "SDL2_LIBRARY: " ${SDL2_LIBRARY})
ELSE()
    Message( STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    Message( FATAL_ERROR "SDL2 NOT FOUND" )
ENDIF()

FIND_PACKAGE(OpenCL REQUIRED)
Message("")
Message( STATUS "FINDING OpenCL" )
IF (${OpenCL_FOUND})
    Message( STATUS "OpenCL_FOUND: " ${OpenCL_FOUND})
    Message( STATUS "OpenCL_INCLUDE_DIRS: " ${OpenCL_INCLUDE_DIRS})
    Message( STATUS "OpenCL_LIBRARIES: " ${OpenCL_LIBRARIES})
ELSE()
    Message( STATUS "OpenCL_FOUND: " ${OpenCL_FOUND})
    Message( FATAL_ERROR "OpenCL NOT FOUND" )
ENDIF()

# file(COPY resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# ------- End Finds ------ #

# ------- Inc & Link ---- #

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR} ${OpenCL_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARY} ${OpenCL_LIBRARIES})

# ------- Copy resources next to the executable ------- #

# Plain POST_BUILD custom commands only re-run when the target itself gets
# relinked, so they'd miss edits to opencl_kernel.cl or ressources/ that don't
# touch any C++ source. Use a proper OUTPUT/DEPENDS rule instead so these are
# re-copied on every build whenever the source files actually changed.
set(KERNEL_STAMP ${CMAKE_BINARY_DIR}/bin/opencl_kernel.cl)
add_custom_command(OUTPUT ${KERNEL_STAMP}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/src/opencl_kernel.cl
        ${KERNEL_STAMP}
        DEPENDS ${CMAKE_SOURCE_DIR}/src/opencl_kernel.cl)

# Game code loads bitmaps from "../ressources" relative to the executable
# (i.e. one directory above CMAKE_RUNTIME_OUTPUT_DIRECTORY), so mirror that here.
set(RESSOURCES_STAMP ${CMAKE_BINARY_DIR}/ressources.stamp)
file(GLOB_RECURSE RESSOURCE_FILES ${CMAKE_SOURCE_DIR}/ressources/*)
add_custom_command(OUTPUT ${RESSOURCES_STAMP}
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/ressources
        ${CMAKE_BINARY_DIR}/ressources
        COMMAND ${CMAKE_COMMAND} -E touch ${RESSOURCES_STAMP}
        DEPENDS ${RESSOURCE_FILES})

add_custom_target(CopyGameData ALL DEPENDS ${KERNEL_STAMP} ${RESSOURCES_STAMP})
add_dependencies(${PROJECT_NAME} CopyGameData)

# ------- End ----------- #



