cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)

include(source/CMakeLists.txt)
foreach(FILE ${FILES})
  set(sourceFiles ${sourceFiles} source/${FILE})
endforeach()

add_executable(buck_wood ${sourceFiles})

target_include_directories(buck_wood PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/source/framework
  ${CMAKE_CURRENT_SOURCE_DIR}/source/collisions
  ${CMAKE_CURRENT_SOURCE_DIR}/source/geometry
  ${CMAKE_CURRENT_SOURCE_DIR}/source/level_container
  ${CMAKE_CURRENT_SOURCE_DIR}/source/level_data
  ${CMAKE_CURRENT_SOURCE_DIR}/source/play_screen
  ${CMAKE_CURRENT_SOURCE_DIR}/source/renderers
  ${CMAKE_CURRENT_SOURCE_DIR}/source/user_interface
)
