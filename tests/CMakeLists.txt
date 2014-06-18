# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules
find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets Test)

# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

message("[INFO] tests")
message("[INFO] CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
message("[INFO] CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
message("[INFO] CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}")

include_directories(
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/generated-include
  ${CMAKE_CURRENT_BINARY_DIR}
)

# Test: Test1
set(Test1Src Test1.cpp)
add_executable(Test1 ${Test1Src})
target_link_libraries(Test1 Qt5::Core Qt5::Test kreenshoteditor)
add_test(NAME Test1 COMMAND Test1)

# Test: TestCore
set(TestCoreSrc TestCore.cpp)
add_executable(TestCore ${TestCoreSrc})
target_link_libraries(TestCore Qt5::Core Qt5::Test kreenshoteditor)
add_test(NAME TestCore COMMAND TestCore)

# Test: TestSettings
set(TestSettingsSrc TestSettings.cpp)
add_executable(TestSettings ${TestSettingsSrc})
target_link_libraries(TestSettings Qt5::Core Qt5::Test kreenshoteditor)
add_test(NAME TestSettings COMMAND TestSettings)

# Test: TestUi
set(TestUiSrc TestUi.cpp)
add_executable(TestUi ${TestUiSrc})
target_link_libraries(TestUi Qt5::Core Qt5::Test kreenshoteditor)
add_test(NAME TestUi COMMAND TestUi)

# http://stackoverflow.com/questions/13429656/cmake-how-to-copy-contents-of-a-directory-into-build-directory-after-make
# TODO: cleanup dir before copying stuff
add_custom_command(TARGET TestCore PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/testdata $<TARGET_FILE_DIR:TestCore>/testdata)

# kate: indent-width 4; replace-tabs on;
