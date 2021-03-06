project(kreenshoteditor)
message("[INFO] lib/kreenshoteditor/start")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules

find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets Network)

include_directories(
    .
)

set(kreenshoteditor_SOURCES
    core/impl/outputfilenamegenerator.cpp
    core/impl/undocommands.cpp

    core/desktopservices.cpp
    core/document.cpp
    core/documentfile.cpp
    core/kreenitem.cpp
    core/settingsgroupoutput.cpp
    core/settingsmanager.cpp

    ui/cursors/cursorlightcross.cpp

    ui/impl/kreengraphicsitembase.cpp
    ui/impl/kreengraphicsitems.cpp
    ui/impl/kreengraphicsscene.cpp
    ui/impl/kreengraphicsview.cpp
    ui/impl/rendervisibilitycontrol.cpp

    ui/selectionhandles/selectionhandlebase.cpp
    ui/selectionhandles/selectionhandlegraphicsitem.cpp
    ui/selectionhandles/selectionhandlesmgr.cpp

    ui/settings/pageoutput.cpp
    ui/settings/preferencesdialog.cpp

    ui/widgets/kreentoolbutton.cpp

    ui/maineditorwidget.cpp

    kreenshoteditor.cpp
)

set(kreenshoteditor_FORMS
    ui/settings/preferencesdialog.ui
    ui/settings/pageoutput.ui
    )
message("[DEBUG] kreenshoteditor_FORMS=${kreenshoteditor_FORMS}")
qt5_wrap_ui(kreenshoteditor_FORMS_HEADERS ${kreenshoteditor_FORMS})
message("[DEBUG] kreenshoteditor_FORMS_HEADERS=${kreenshoteditor_FORMS_HEADERS}")

add_library(kreenshoteditor SHARED
    ${kreenshoteditor_SOURCES}
    ${kreenshoteditor_FORMS_HEADERS}
)

target_link_libraries(kreenshoteditor
     Qt5::Core
     Qt5::Widgets
     Qt5::Network
)

# see http://public.kitware.com/pipermail/cmake/2013-July/055375.html
# [CMake] How can I place a 64bit library build into /usr/lib64?
get_property(LIB64__ GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
if(LIB64__)
    set(LIB_SUFFIX 64)
else()
    set(LIB_SUFFIX "")
endif()
message("[DEBUG] LIB_SUFFIX=${LIB_SUFFIX}")
message("[DEBUG] env var XDG_DATA_DIRS=$ENV{XDG_DATA_DIRS}")
message("[DEBUG] env var from spec file KREEN_SPEC_ARCH=$ENV{KREEN_SPEC_ARCH}")
if ("$ENV{KREEN_SPEC_ARCH}" STREQUAL "i386") # see export KREEN_SPEC_ARCH=%{_arch} in spec file
    set(LIB_SUFFIX "")
endif()
message("[DEBUG] after WORKAROUND LIB_SUFFIX=${LIB_SUFFIX}")

##### START: header file handling #####
#####
    # http://stackoverflow.com/questions/15972898/cmake-how-to-run-a-add-custom-command-before-everything-else
    message("[INFO] src/lib/kreenshoteditor add_custom_target: prepare-include-files")
    add_custom_target(prepare-include-files
        # COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/mk_config_h.py ${CMAKE_CURRENT_BINARY_DIR}/config.h
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/prepare-include-files.sh generated-include WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}

        # when outside add_custom_target use execute_process but this is only run when cmake is configuring:
        # http://stackoverflow.com/questions/1438535/how-to-run-a-command-at-compile-with-in-makefile-generated-by-cmake
        # execute_process(COMMAND echo "Hallo")
    )
    add_dependencies(kreenshoteditor prepare-include-files)
#####
##### END: header file handling #####

install(TARGETS kreenshoteditor
  LIBRARY DESTINATION lib${LIB_SUFFIX}
)

message("[INFO] lib/kreenshoteditor/end")

# kate: indent-width 4; replace-tabs on;
