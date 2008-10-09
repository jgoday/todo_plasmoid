set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules )

# search packages used by KDE
find_package(KDE4 REQUIRED)  
find_package(Plasma REQUIRED)
include (KDE4Defaults)       
include (MacroLibrary)       
include(MacroOptionalAddSubdirectory)
include(FindKdepimLibs) 

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} ${CMAKE_SOURCE_DIR}/libkdepim/)

add_subdirectory(src/applet)
add_subdirectory(src/dataengine)
