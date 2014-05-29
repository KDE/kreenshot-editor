project(kreenshoteditorapp)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules
find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets)

include_directories(
    .
    ui
    # ../libkreenshoteditor # TODO:
    # TODO: later:
    ${CMAKE_SOURCE_DIR}/src/generated-include
)

set(kreenshoteditorapp_SOURCES
    main.cpp
    ui/mainwindow.cpp
)

set(kreenshoteditorapp_FORMS ui/mainwindow.ui)
qt5_wrap_ui(kreenshoteditorapp_FORMS_HEADERS ${kreenshoteditorapp_FORMS})

add_definitions("-std=c++11")

add_executable(kreenshot-editor
    ${kreenshoteditorapp_SOURCES}
    ${kreenshoteditorapp_FORMS_HEADERS}
)

target_link_libraries(kreenshot-editor
    Qt5::Core
    Qt5::Widgets
    kreenshoteditor
)

add_subdirectory(lib/kreenshoteditor)

# kate: indent-width 4; replace-tabs on;
