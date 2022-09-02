
#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(PROJECT_B1)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
set(useROOT false)
set(B1_SOURCE_DIRECTORIES src)
set(B1_EXECUTABLES example)
set(B1_SCRIPTS vis.mac)

#----------------------------------------------------------------------------
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

if(useROOT)
  list(APPEND B1_SOURCE_DIRECTORIES)
  list(APPEND B1_EXECUTABLES)
endif(useROOT)

set(B1_SOURCES_DIRECTORIES_FULL)
foreach(_directory ${B1_SOURCE_DIRECTORIES})
  list(APPEND B1_SOURCE_DIRECTORIES_FULL ${PROJECT_SOURCE_DIR}/${_directory})
endforeach(_directory)

set(B1_SOURCES)
foreach(_directory ${B1_SOURCE_DIRECTORIES_FULL})
  list(APPEND B1_SOURCES ${_directory}/*.cc)
endforeach(_directory)

set(B1_HEADERS)
foreach(_directory ${B1_SOURCE_DIRECTORIES_FULL})
  list(APPEND B1_HEADERS ${_directory}/*.hh)
endforeach(_directory)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${B1_SOURCE_DIRECTORIES_FULL})

if(useROOT)
  EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
endif(useROOT)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${B1_SOURCES})
file(GLOB headers ${B1_HEADERS})

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
foreach(_executable ${B1_EXECUTABLES})
  add_executable(${_executable} ${_executable}.cc ${sources} ${headers})
if(useROOT)
  EXECUTE_PROCESS(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LD_FLAGS})
endif(useROOT)
  target_link_libraries(${_executable} ${Geant4_LIBRARIES})
endforeach(_executable)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
foreach(_script ${B1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach(_script)

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(PROJECT_B1 DEPENDS ${B1_EXECUTABLES})

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
foreach(_executable ${B1_EXECUTABLES})
  install(TARGETS ${_executable} DESTINATION bin)
endforeach(_executable)


