#=========================================================
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7 FATAL_ERROR)

#=========================================================
#AngioTk = Virtual Angiography Toolkit
PROJECT(AngioTk)
#=========================================================

#=========================================================
#Support for the CTest dashboard testing system
INCLUDE(CTest)
#=========================================================

IF(BUILD_TESTING)
  ENABLE_TESTING()
  INCLUDE(CMake/AngioTkExternalData.cmake)
  
  # Sets the data root
  set(AngiotTk_DATA_ROOT ${AngioTk_SOURCE_DIR}/Testing/Data)
  mark_as_advanced(AngiotTk_DATA_ROOT)
ENDIF(BUILD_TESTING)
#=========================================================

#=========================================================
SET(AngioTk_VERSION_MAJOR "0")
SET(AngioTk_VERSION_MINOR "1")
SET(AngioTk_VERSION_PATCH "0")

#=========================================================

#=========================================================
# Output directories.
IF(NOT LIBRARY_OUTPUT_PATH)
  SET (LIBRARY_OUTPUT_PATH ${AngioTk_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all libraries.")
ENDIF(NOT LIBRARY_OUTPUT_PATH)
IF(NOT EXECUTABLE_OUTPUT_PATH)
  SET (EXECUTABLE_OUTPUT_PATH ${AngioTk_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
ENDIF(NOT EXECUTABLE_OUTPUT_PATH)
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

SET(AngioTk_LIBRARY_PATH "${LIBRARY_OUTPUT_PATH}")
SET(AngioTk_EXECUTABLE_PATH "${EXECUTABLE_OUTPUT_PATH}")
set(AngioTk_TEST_OUTPUT_DIR "${AngioTk_BINARY_DIR}/Testing/Temporary")
SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

#=========================================================
# Remove some MS Visual c++ flags
IF(MSVC)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
ENDIF(MSVC)

# Flag required for shared libraries
IF ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
  SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
#=========================================================

#=========================================================
# Find ITK (required)
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
  if(${ITK_VERSION_MAJOR} EQUAL 3)
    message(FATAL_ERROR "AngioTk only works with ITK version 4 and higher")
  endif(${ITK_VERSION_MAJOR} EQUAL 3)
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR "Cannot build without ITK. Set ITK_DIR.")
ENDIF(ITK_FOUND)

#=========================================================
# Find Feel++
FIND_PATH(FEELPP_CMAKE_MODULES FindFeel++.cmake PATH $ENV{FEELPP_DIR}/share/feel/cmake/modules/   /usr/share/feel/cmake/modules/ /usr/local/share/feel/cmake/modules/ )
if ( FEELPP_CMAKE_MODULES )
  set(CMAKE_MODULE_PATH ${FEELPP_CMAKE_MODULES})
  Find_Package(Feel++)
  set(Feel++_FOUND 1)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g0 -O3 -DNDEBUG  -finline-functions -pedantic  -ftemplate-depth-256 -Wno-inline" )
else()
  #message(FATAL_ERROR "Feel++ does not seem to have been installed on this platform")
endif()

#=========================================================
# Find Python
FIND_PACKAGE(PythonLibs)
FIND_PACKAGE(PythonInterp)

#=========================================================
# Find VMTK
FIND_PATH(VMTK_BINARY_DIR NAMES vmtkcenterlines PATH $ENV{VMTK_DIR}/bin )
# if ( NOT VMTK_BINARY_DIR )
#   message(FATAL_ERROR "VMTK_BINARY_DIR not find")
# endif()



#=========================================================
# Shared libraries option
OPTION(BUILD_SHARED_LIBS "Build AngioTk with shared libraries." ${ITK_BUILD_SHARED})
SET(AngioTk_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
#=========================================================

#=========================================================
# AngioTk uses KWStyle for checking the coding style
INCLUDE(${AngioTk_SOURCE_DIR}/CMake/KWStyle/KWStyle.cmake)
#=========================================================


#=========================================================
# If choose to build documentation, then search for Doxygen executables.
OPTION(BUILD_DOXYGEN "Build Doxygen Documentation" OFF)
IF(BUILD_DOXYGEN)
  ADD_SUBDIRECTORY(documentation/Doxygen)
ENDIF(BUILD_DOXYGEN)
#=========================================================

#=========================================================
# If choose to build examples, then look into Examples folder.
OPTION(BUILD_EXAMPLES "Build examples" OFF)
IF(BUILD_EXAMPLES)
  ADD_SUBDIRECTORY(Examples/Python)
ENDIF(BUILD_EXAMPLES)
#=========================================================

#=========================================================
# Installation variables
if(NOT AngioTK_INSTALL_RUNTIME_DIR)
  set(AngioTK_INSTALL_RUNTIME_DIR bin)
endif()
if(NOT AngioTK_INSTALL_LIB_DIR)
  set(AngioTK_INSTALL_LIB_DIR lib)
endif()
if(NOT AngioTK_INSTALL_ARCHIVE_DIR)
  set(AngioTK_INSTALL_ARCHIVE_DIR lib)
endif()
if(NOT AngioTK_INSTALL_INCLUDE_DIR)
  set(AngioTK_INSTALL_INCLUDE_DIR include/AngioTk-${AngioTK_VERSION_MAJOR}.${AngioTK_VERSION_MINOR})
endif()
if(NOT AngioTK_INSTALL_PACKAGE_DIR)
  set(AngioTK_INSTALL_PACKAGE_DIR "lib/cmake/AngioTk-${AngioTK_VERSION_MAJOR}.${AngioTK_VERSION_MINOR}")
endif()
#=========================================================

INCLUDE_DIRECTORIES(${AngioTK_INCLUDE_DIRS})

# Useful generic macros for modules configuration  
INCLUDE("${AngioTk_SOURCE_DIR}/CMake/AngioTkMacros.cmake")

# Initiate modules configuration
INCLUDE("${AngioTk_SOURCE_DIR}/CMake/AngioTkModules.cmake")

ExternalData_Add_Target(angioTkData)

