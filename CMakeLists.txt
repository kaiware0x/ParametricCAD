cmake_minimum_required(VERSION 3.5)

project(ParametricCAD VERSION 0.1 LANGUAGES CXX)

include(Utility.cmake)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg)

getSrcFiles() # Utility.cmake
excludeSrcDir(mymod) # Utility.cmake
excludeSrcDir(PythonConsole) # Utility.cmake

qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
    README.md
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Svg
    PythonConsole
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(${PROJECT_NAME})

setOutputDir(${PROJECT_NAME}) # Utility.cmake
linkPybind11ForEmbed(${PROJECT_NAME}) # Utility.cmake
linkPython(${PROJECT_NAME}) # Utility.cmake
linkOpenCASCADE(${PROJECT_NAME})

add_subdirectory(PythonConsole/)
add_subdirectory(mymod/)
