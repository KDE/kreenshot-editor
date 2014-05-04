project(kreenshot-editor)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules
find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets)

include_directories(
    .
    ui
    # TODO: later:
    ${CMAKE_SOURCE_DIR}/src/generated-include
)

set(kreenshot-editor_SOURCES
    main.cpp
    ui/MainWindow.cpp
)

set(kreenshot-editor_FORMS ui/MainWindow.ui)
qt5_wrap_ui(kreenshot-editor_FORMS_HEADERS ${kreenshot-editor_FORMS})

add_definitions("-std=c++11")

add_executable(kreenshot-editor
    ${kreenshot-editor_SOURCES}
    ${kreenshot-editor_FORMS_HEADERS}
)

target_link_libraries(kreenshot-editor
    Qt5::Core
    Qt5::Widgets
    #qtikzcore
    #qtikzui
)

# add_subdirectory(libkreenshoteditor)

# kate: indent-width 4; replace-tabs on;
