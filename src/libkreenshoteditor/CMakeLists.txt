# project(kreenshot-editor-ui)
#
# # Find includes in corresponding build directories
# set(CMAKE_INCLUDE_CURRENT_DIR ON)
#
# # Instruct CMake to run moc automatically when needed.
# set(CMAKE_AUTOMOC ON)
#
# # find Qt5 modules
# find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets)
#
# include_directories(
#     .
# )
#
# set(kreenshot-editor-ui_SOURCES
#     MainWindow.cpp
# )
#
# set(kreenshot-editor-ui_FORMS MainWindow.ui)
# qt5_wrap_ui(kreenshot-editor-ui_FORMS_HEADERS ${kreenshot-editor-ui_FORMS})
#
# target_link_libraries(kreenshot-editor
#     Qt5::Core
#     Qt5::Widgets
#     #qtikzcore
#     #qtikzui
# )
#
# # kate: indent-width 4; replace-tabs on;
