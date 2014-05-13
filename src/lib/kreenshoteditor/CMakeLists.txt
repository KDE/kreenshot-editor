project(kreenshoteditor)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules
find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets)

include_directories(
    .
)

set(kreenshoteditor_SOURCES
    kreenshoteditor.cpp
    ui/maineditorwidget.cpp
    ui/myqgraphicsview.cpp
    ui/kreengraphicsitems.cpp
    core/item.cpp
    core/itemsmanager.cpp
)

# set(kreenshot-editor-ui_FORMS MainWindow.ui)
# qt5_wrap_ui(kreenshot-editor-ui_FORMS_HEADERS ${kreenshot-editor-ui_FORMS})

add_library(kreenshoteditor SHARED
    ${kreenshoteditor_SOURCES}
#    tikzmoc
)

target_link_libraries(kreenshoteditor
     Qt5::Core
     Qt5::Widgets
)

# kate: indent-width 4; replace-tabs on;
