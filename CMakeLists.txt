CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5 FATAL_ERROR)
cmake_policy(SET CMP0003 NEW)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

################################################################################
# Set default values
################################################################################

PROJECT(SACRED_FRACTAL)
SET(CMAKE_CXX_FLAGS_DEBUG "-fopenmp -O0 -g2 -pg -fno-inline-functions -fno-inline")
SET(CMAKE_C_FLAGS_DEBUG "-fopenmp -O0 -g2 -pg -fno-inline-functions -fno-inline")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

SET(OSG_DIR "${CMAKE_INSTALL_PREFIX}" CACHE STRING "Path where to find the osg installation")

#######################################
# Versions
#######################################

SET(SACRED_FRACTAL_MAJOR_VERSION 0)
SET(SACRED_FRACTAL_MINOR_VERSION 0)
SET(SACRED_FRACTAL_PATCH_VERSION 1)

SET(SACRED_FRACTAL_RELEASE_CANDIDATE 0)

SET(SACRED_FRACTAL_VERSION ${SACRED_FRACTAL_MAJOR_VERSION}.${SACRED_FRACTAL_MINOR_VERSION}.${SACRED_FRACTAL_PATCH_VERSION})
SET(SACRED_FRACTAL_SONAMES TRUE)

#######################################
# Library files and dependencies
#######################################

FIND_PACKAGE( OpenGL REQUIRED )
FIND_PACKAGE( OSG    REQUIRED )
FIND_PACKAGE( OpenAL REQUIRED )
FIND_PACKAGE( FFTW   REQUIRED ) 
FIND_PACKAGE( OpenVR REQUIRED )

include(CheckCXXCompilerFlag)
if (NOT MSVC)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 COMPILER_SUPPORTS_CXX17)
  CHECK_CXX_COMPILER_FLAG(-std=c++0x COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG(-std=c++0x COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  elseif(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
  endif()
endif()

################################################################################
# Make the headers visible to everything
################################################################################

IF(NOT ${PROJECT_BINARY_DIR} EQUAL ${PROJECT_SOURCE_DIR})
   INCLUDE_DIRECTORIES(BEFORE ${PROJECT_BINARY_DIR}/src)
ENDIF(NOT ${PROJECT_BINARY_DIR} EQUAL ${PROJECT_SOURCE_DIR})

INCLUDE_DIRECTORIES(BEFORE
    ${OPENGL_INCLUDE_DIR}
    ${OSG_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIR}
    ${SOURCE_DIR}/src
)

SET(GL_LIBS
  ${OPENGL_gl_LIBRARY}
  ${OPENGL_glu_LIBRARY}
)

################################################################################
#Solution
################################################################################

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build)
SET(WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/build)

SET(EXAMPLE_PREFIX sacred_ CACHE STRING "Prefix used to build examples")
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

file(GLOB_RECURSE GUI_SRC      "src/gui/*.h"      "src/gui/*.cpp")
file(GLOB_RECURSE GRAPHICS_SRC "src/graphics/*.h" "src/graphics/*.cpp")
file(GLOB_RECURSE SOUNDS_SRC   "src/sounds/*.h"   "src/sounds/*.cpp")
file(GLOB_RECURSE OPENVR_SRC   "src/openvr/*.h"   "src/openvr/*.cpp")

SET(GL_LIBS      GL glut)
SET(OSG_LIBS     osg osgViewer osgDB osgGA)
SET(SACRED_LIBS  sacred_gui sacred_sounds sacred_graphics sacred_vr)
SET(SOUND_LIBS   fftw3 fftw3f openal sndfile)
SET(ALL_LIBS     ${SACRED_LIBS} ${SOUND_LIBS} ${CMAKE_DL_LIBS} ${GL_LIBS} ${OSG_LIBS})

include_directories(src/)
include_directories(src/gui/imgui)
include_directories(src/test)

add_library(sacred_sounds   STATIC ${SOUNDS_SRC})
add_library(sacred_graphics STATIC ${GRAPHICS_SRC})
add_library(sacred_gui      STATIC ${GUI_SRC})
add_library(sacred_vr       STATIC ${OPENVR_SRC})

target_link_libraries(sacred_gui  ${OSG_LIBS})
target_link_libraries(sacred_graphics sacred_sounds)
target_link_libraries(sacred_vr ${OPENVR_SDK_LIBRARY}  ${OSG_LIBS})

add_executable(sacred src/main.cpp)
target_include_directories(sacred PUBLIC src/)
target_link_libraries(sacred ${ALL_LIBS})


####################################################
#                       TEST                       #
####################################################

add_executable(openal_test src/test/openal_test.cpp)
set_target_properties(openal_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_include_directories(openal_test PUBLIC src/)
target_link_libraries(openal_test ${ALL_LIBS})

add_executable(track_test src/test/track_test.cpp)
set_target_properties(track_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_include_directories(track_test PUBLIC src/)
target_link_libraries(track_test ${ALL_LIBS})

add_executable(spectrum_test src/test/spectrum_test.cpp)
set_target_properties(spectrum_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_include_directories(spectrum_test PUBLIC src/)
target_link_libraries(spectrum_test ${ALL_LIBS})

add_executable(dissonance_test src/test/dissonance_test.cpp)
set_target_properties(dissonance_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_include_directories(dissonance_test PUBLIC src/)
target_link_libraries(dissonance_test ${ALL_LIBS})

add_executable(dynamics_test src/test/dynamics_test.cpp)
set_target_properties(dynamics_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_include_directories(dynamics_test PUBLIC src/)
target_link_libraries(dynamics_test ${ALL_LIBS})

## operator test

add_executable(additioner_test src/test/operator/additioner_test.cpp)
set_target_properties(additioner_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_include_directories(additioner_test PUBLIC src/)
target_link_libraries(additioner_test ${ALL_LIBS})

add_executable(multiplier_test src/test/operator/multiplier_test.cpp)
set_target_properties(multiplier_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_include_directories(multiplier_test PUBLIC src/)
target_link_libraries(multiplier_test ${ALL_LIBS})

add_executable(substractor_test src/test/operator/substractor_test.cpp)
set_target_properties(substractor_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_include_directories(substractor_test PUBLIC src/)
target_link_libraries(substractor_test ${ALL_LIBS})

## generator test

add_executable(sinegen_test src/test/generator/sinegen_test.cpp)
set_target_properties(sinegen_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_include_directories(sinegen_test PUBLIC src/)
target_link_libraries(sinegen_test ${ALL_LIBS})

add_executable(sawgen_test src/test/generator/sawgen_test.cpp)
set_target_properties(sawgen_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_include_directories(sawgen_test PUBLIC src/)
target_link_libraries(sawgen_test ${ALL_LIBS})

add_executable(squaregen_test src/test/generator/squaregen_test.cpp)
set_target_properties(squaregen_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_include_directories(squaregen_test PUBLIC src/)
target_link_libraries(squaregen_test ${ALL_LIBS})

add_executable(trianglegen_test src/test/generator/trianglegen_test.cpp)
set_target_properties(trianglegen_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_include_directories(trianglegen_test PUBLIC src/)
target_link_libraries(trianglegen_test ${ALL_LIBS})

add_executable(bouncegen_test src/test/generator/bouncegen_test.cpp)
set_target_properties(bouncegen_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_include_directories(bouncegen_test PUBLIC src/)
target_link_libraries(bouncegen_test ${ALL_LIBS})

## add to ctest

enable_testing()
add_test(openal_test      ./test/openal_test)
add_test(track_test       ./test/track_test)
add_test(additioner_test  ./test/additioner_test)
add_test(multiplier_test  ./test/multiplier_test)
add_test(substractor_test ./test/substractor_test)
add_test(spectrum_test    ./test/spectrum_test)
add_test(dissonance_test  ./test/dissonance_test)
add_test(sinegen_test     ./test/sinegen_test)
add_test(sawgen_test      ./test/sawgen_test)
add_test(squaregen_test   ./test/squaregen_test)
add_test(trianglegen_test ./test/trianglegen_test)
add_test(bouncegen_test   ./test/bouncegen_test)
add_test(dynamics_test    ./test/dynamics_test)

################################################################################
# Create bin and lib directories if required
################################################################################

#IF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
#   FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/build
#ENDIF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
