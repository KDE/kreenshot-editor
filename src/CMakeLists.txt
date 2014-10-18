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
# see http://stackoverflow.com/questions/3486987/error-on-missing-return-statement
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror=return-type -fdiagnostics-show-option")

# see http://www.cmake.org/pipermail/cmake/2011-May/044451.html
# set(CMAKE_VERBOSE_MAKEFILE true) # does not work in kdevelop

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

##### start: header file handling #####

    # http://stackoverflow.com/questions/15972898/cmake-how-to-run-a-add-custom-command-before-everything-else
    message("[INFO] src/add_custom_target: prepare-include-files")
    add_custom_target(prepare-include-files
        # COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/mk_config_h.py ${CMAKE_CURRENT_BINARY_DIR}/config.h
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/prepare-include-files.sh generated-include WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}

        # when outside add_custom_target use execute_process but this is only run when cmake is configuring:
        # http://stackoverflow.com/questions/1438535/how-to-run-a-command-at-compile-with-in-makefile-generated-by-cmake
        # execute_process(COMMAND echo "Hallo")
    )
    add_dependencies(kreenshot-editor prepare-include-files)

##### end: header file handling #####

# see http://www.cmake.org/Wiki/CMake:Install_Commands
# and lib/kreenshoteditor/CMakeLists.txt
install(TARGETS kreenshot-editor
  RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/XDG_DATA_DIR/kreenshot-editor
    DESTINATION share
    PATTERN ".*" EXCLUDE
)

message("[INFO] src/end")

# kate: indent-width 4; replace-tabs on;
