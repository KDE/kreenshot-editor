project(kreenshoteditorapp)
message("[INFO] src/start")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules
message("[INFO] src/find_package Qt5")
find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets)

message("[INFO] src/include_directories")
include_directories(
    .
    ui
    # see prepare-include-files.sh;
    # Reason: instead of #include "../core/item.h"
    #         we would like to write #include "kreen/core/item.h"
    ${CMAKE_SOURCE_DIR}/src/generated-include
)

set(kreenshoteditorapp_SOURCES
    main.cpp
    ui/mainwindow.cpp
)

set(kreenshoteditorapp_FORMS ui/mainwindow.ui)
qt5_wrap_ui(kreenshoteditorapp_FORMS_HEADERS ${kreenshoteditorapp_FORMS})

add_definitions("-std=c++11")

message("[INFO] src/add_executable")
add_executable(kreenshot-editor
    ${kreenshoteditorapp_SOURCES}
    ${kreenshoteditorapp_FORMS_HEADERS}
)

message("[INFO] src/target_link_libraries")
target_link_libraries(kreenshot-editor
    Qt5::Core
    Qt5::Widgets
    kreenshoteditor
)

message("[INFO] src/add_subdirectory(lib/kreenshoteditor)")
add_subdirectory(lib/kreenshoteditor)

# http://stackoverflow.com/questions/1438535/how-to-run-a-command-at-compile-with-in-makefile-generated-by-cmake
message("[INFO] src/execute_process: prepare-include-files.sh")
execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/prepare-include-files.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
# execute_process(COMMAND echo "Hallo")

message("[INFO] src/end")

# kate: indent-width 4; replace-tabs on;
