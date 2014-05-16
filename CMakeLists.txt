#
# The official cmake documentation for Qt5 is available here:
# http://qt-project.org/doc/qt-5/cmake-manual.html
#
cmake_minimum_required(VERSION 2.8.11)

project(kreenshot-editor)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules
)

add_definitions("-std=c++11")

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
