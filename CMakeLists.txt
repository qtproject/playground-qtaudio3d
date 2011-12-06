#################################################################################
# This file is part of the QtOpenAL project
# Copyright (c) 2011 Laszlo Papp <lpapp@kde.org>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#/

cmake_minimum_required(VERSION 2.8)
project(QtOpenAL)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(OpenAL)
if(NOT OPENAL_FOUND)
    message(FATAL_ERROR "OpenAL not found!\nPlease try setting the OPENALDIR environment variable and try again.")
endif(NOT OPENAL_FOUND)

message(STATUS "OpenAL include: ${OPENAL_INCLUDE_DIR}")
message(STATUS "OpenAL lib: ${OPENAL_LIBRARY}")

find_package(OggVorbis)
if(OGGVORBIS_FOUND)
    message(STATUS "VorbisFile support: enabled")
else(OGGVORBIS_FOUND)
    message(STATUS "VorbisFile support: disabled")
endif(OGGVORBIS_FOUND)

find_package(SndFile)
if(SNDFILE_FOUND)
    message(STATUS "SndFile support: enabled")
else(SNDFILE_FOUND)
    message(STATUS "SndFile support: disabled")
endif(SNDFILE_FOUND)

INCLUDE_DIRECTORIES(${OPENAL_INCLUDE_DIR} ${SNDFILE_INCLUDE_DIRS})
SET(OPENAL_LIBRARIES ${OPENAL_LIBRARY})

#Project options, to provide a selective build process
#Note that some of these are mutually exclusive or just
#do not make a whole lot of sense combined.

if(CMAKE_INSTALL_PREFIX STREQUAL "/usr")
    option(INSTALL_CMAKE_FILES "Install the *.cmake files into the CMake root" ON)
else()
    option(INSTALL_CMAKE_FILES "Install the *.cmake files into the CMake root" OFF)
endif()

SET(CMAKE_CXX_FLAGS "-Wall")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -O2 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O2")
SET(CMAKE_CXX_FLAGS_DEBUG  "-Wall -g3 -ggdb -O0" CACHE STRING "Debug options." FORCE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

####### Source #######
add_subdirectory(src)

message(STATUS)
message(STATUS "========== QtOpenAL Build Information ==========")
message(STATUS "Build Version: ${QTOPENAL_VERSION_STRING} (${QTOPENAL_VERSION_NAME})")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Install the *.cmake files into CMake root (INSTALL_CMAKE_FILES): ${INSTALL_CMAKE_FILES}")
message(STATUS)
message(STATUS "To change any of these options, override them using -D{OPTION_NAME} on the commandline.")
message(STATUS "To build and install QtOpenAL, run \"make\" and \"make install\"")
message(STATUS)

if(INSTALL_CMAKE_FILES)
    install(
        FILES
        DESTINATION
        ${CMAKE_ROOT}/Modules
        OPTIONAL
    )
else()
    install(
        FILES
        DESTINATION
        ${SHARE_INSTALL_DIR}/qtopenal/cmake
        OPTIONAL
    )
endif()

###### CPACK part ######
include(CPack)

###### CTEST part ######
include(CTest)

