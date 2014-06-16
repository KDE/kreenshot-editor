#
# The official cmake documentation for Qt5 is available here:
# http://qt-project.org/doc/qt-5/cmake-manual.html
#
cmake_minimum_required(VERSION 2.8.11)

message("[INFO] root/start")

project(kreenshot-editor)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules
)

add_definitions("-std=c++11")
# see http://stackoverflow.com/questions/3486987/error-on-missing-return-statement
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror=return-type -fdiagnostics-show-option")

message("[INFO] root/add_subdirectory(src)")
add_subdirectory(src)

enable_testing()
message("[INFO] root/add_subdirectory(tests)")
add_subdirectory(tests)

message("[INFO] root/end")