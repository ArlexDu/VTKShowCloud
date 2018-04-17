cmake_minimum_required(VERSION 3.8)
foreach (p
        CMP0071 # 3.10: Let AUTOMOC and AUTOUIC process GENERATED files
        )
    if (POLICY ${p})
        cmake_policy(SET ${p} NEW)
    endif ()
endforeach ()

project(VTKShowCloud)

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
if ("${VTK_QT_VERSION}" STREQUAL "")
    message(FATAL_ERROR "VTK was not built with Qt")
endif ()


# Use the include path and library for Qt that is used by VTK.
include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set your files and resources here
set(Srcs main.cpp mainwindow.cpp HS_Lidar.cpp vtkLASReader.cpp)

set(Hdrs mainwindow.h HS_Lidar_Header.h HS_Lidar_Channel.h HS_Lidar.h vtkLASReader.h)

set(MOC_Hdrs mainwindow.h)

set(UIs mainwindow.ui)

set(QRCs Resources/resources.qrc)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# The rest should just work (sure...)
# We have ui files, this will bring in the macro: qt5_wrap_ui
find_package(Qt5 COMPONENTS Core Gui Widgets Charts REQUIRED QUIET)
qt5_wrap_ui(UI_Srcs ${UIs})
qt5_add_resources(QRC_Srcs ${QRCs})

source_group("Resources" FILES
        ${UIs}
        ${QRCs}
        ${EXE_ICON} # Not present
        )

source_group("Generated" FILES
        ${UI_Srcs}
        ${MOC_Srcs}
        ${QRC_Srcs}
        )

#Add Executable
add_executable(VTKShowCloud ${Srcs} ${Hdrs} ${UI_Srcs} ${MOC_Hdrs} ${QRC_Srcs})
#add_executable(VTKShowCloud MACOSX_BUNDLE ${Srcs} ${Hdrs} ${UI_Srcs} ${MOC_Hdrs} ${QRC_Srcs})

qt5_use_modules(VTKShowCloud Core Gui Widgets Charts)

target_link_libraries(VTKShowCloud ${PCL_LIBRARIES} ${libLAS_LIBRARIES} ${VTK_LIBRARIES})
