project(kreenshoteditor)

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
    core/item.cpp
    core/itemsmanager.cpp
    core/outputfilenamemanager.cpp

    ui/kreengraphicsitems.cpp
    ui/maineditorwidget.cpp
    ui/myqgraphicsscene.cpp
    ui/myqgraphicsview.cpp

    kreenshoteditor.cpp
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
     Qt5::Network
)

# kate: indent-width 4; replace-tabs on;
