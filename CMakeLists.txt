cmake_minimum_required(VERSION 3.8)
project(VTKShowCloud)

set(CMAKE_CXX_STANDARD 11)

# set source files
set(SOURCE_FILES main.cpp)
set(GDAL_LIBRARY ${GDAL_LINK_DIRECTORIES}/lib${GDAL_LIB_NAME}.dylib CACHE STRING INTERNAL)

#PCL config
find_package(PCL 1.8 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

#Liblas config
find_package(libLAS 1.8 REQUIRED)
include_directories(${libLAS_INCLUDE_DIRS})
link_directories(${libLAS_LIBRARY_DIRS})

#VTK config
find_package(VTK REQUIRED)
include( ${VTK_USE_FILE} )

#Add Executable
add_executable(Demo ${SOURCE_FILES})

target_link_libraries(Demo ${PCL_LIBRARIES} ${libLAS_LIBRARIES} ${VTK_LIBRARIES})
