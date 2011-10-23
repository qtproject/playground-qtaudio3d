cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Qt4 4.7 REQUIRED)

set(_Init_LIB_SUFFIX "")
if ("${QT_QTCORE_LIBRARY}" MATCHES lib64)
   set(_Init_LIB_SUFFIX 64)
endif ("${QT_QTCORE_LIBRARY}" MATCHES lib64)
if ("${QT_QTCORE_LIBRARY}" MATCHES lib32)
   set(_Init_LIB_SUFFIX 32)
endif ("${QT_QTCORE_LIBRARY}" MATCHES lib32)

set(LIB_SUFFIX		    ${_Init_LIB_SUFFIX} CACHE STRING "The suffix of the system-wide library path")

set(QTOPENAL_VERSION_MAJOR     0       CACHE STRING "QtOpenAL Major Version")
set(QTOPENAL_VERSION_MINOR     1      CACHE STRING "QtOpenAL Minor Version")
set(QTOPENAL_VERSION_PATCH     0       CACHE STRING "QtOpenNAL Patch Version")
set(QTOPENAL_VERSION_STRING
    "${QTOPENAL_VERSION_MAJOR}.${QTOPENAL_VERSION_MINOR}.${QTOPENAL_VERSION_PATCH}" CACHE STRING "QtOpenAL Version String")
set(QTOPENAL_VERSION_NAME      "Electron" CACHE STRING "QtOpenAL Version Name")

set(BIN_INSTALL_DIR         bin                 CACHE INTERNAL  "The subdirectory relative to the install prefix where binary files will be installed.")
set(INCLUDE_INSTALL_DIR     include             CACHE PATH      "The subdirectory relative to the install prefix where header files will be installed.")
set(LIB_INSTALL_DIR         lib${LIB_SUFFIX}    CACHE PATH      "The subdirectory relative to the install prefix where libraries will be installed.")
set(SHARE_INSTALL_DIR       share               CACHE PATH      "The subdiractory relative to the install prefix where data and other files will be installed.")

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( QTOPENAL_ARCHITECTURE "64" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( QTOPENAL_ARCHITECTURE "32" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/qtopenal_global.h.in ${CMAKE_CURRENT_BINARY_DIR}/qtopenal_global.h)

include_directories(${QT_INCLUDES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(QTOPENAL_INCLUDES
    ${QT_INCLUDES}
    CACHE PATHS "Includes required for QtOpenAL"
)

set(QtOpenAL_SRCS
    qalattributes.cpp
    qalbufferformat.cpp
    qalbufferqueue.cpp
    qalcontext.cpp
    qalsource.cpp
)

set(QtOpenAL_HEADERS
    qalattributes.cpp
    qalbufferformat.cpp
    qalbufferqueue.cpp
    qalcontext.cpp
    qalsource.cpp
	${CMAKE_CURRENT_BINARY_DIR}/qtopenal_global.h
)

qt4_automoc(${QtOpenAL_SRCS})

if(APPLE)
    add_library(QtOpenAL SHARED ${QtOpenAL_SRCS} ${QtOpenAL_HEADERS})
else()
    add_library(QtOpenAL SHARED ${QtOpenAL_SRCS})
endif()

target_link_libraries(QtOpenAL ${QT_QTCORE_LIBRARY})

set(QTOPENAL_LIBS
    QtOpenAL
    ${QT_QTCORE_LIBRARY}
    CACHE PATHS "Libraries required for QtOpenAL"
)

set_target_properties(QtOpenAL PROPERTIES VERSION ${QTOPENAL_VERSION_STRING} SOVERSION ${QTOPENAL_VERSION_STRING} DEFINE_SYMBOL MAKE_QTOPENAL_LIB)

install(TARGETS QtOpenAL RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIB_INSTALL_DIR} ARCHIVE DESTINATION ${LIB_INSTALL_DIR})

install(
    FILES
    ${QtOpenAL_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/qtopenal/core
    COMPONENT Devel
)
