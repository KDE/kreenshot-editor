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
    core/impl/kreengraphicsitembase.cpp
    core/impl/kreengraphicsitems.cpp
    core/impl/kreengraphicsscene.cpp
    core/impl/outputfilenamegenerator.cpp
    core/impl/selectionhandlegraphicsitem.cpp
    core/impl/selectionhandles.cpp

    core/document.cpp
    core/documentfile.cpp
    core/kreenitem.cpp
    core/settingsgroupoutput.cpp
    core/settingsmanager.cpp

    ui/impl/kreengraphicsview.cpp

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

message("[INFO] lib/kreenshoteditor/end")

# kate: indent-width 4; replace-tabs on;
