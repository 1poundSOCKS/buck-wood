cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)

include(source/CMakeLists.txt)
foreach(FILE ${FILES})
  set(source ${source} source/${FILE})
endforeach()

include(source/framework/CMakeLists.txt)
foreach(FILE ${FILES})
  set(framework ${framework} source/framework/${FILE})
endforeach()

include(source/collisions/CMakeLists.txt)
foreach(FILE ${FILES})
  set(collisions ${collisions} source/collisions/${FILE})
endforeach()

add_executable(buck_wood ${source} ${framework} ${collisions})

target_include_directories(buck_wood PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/source
  ${CMAKE_CURRENT_SOURCE_DIR}/source/framework
  ${CMAKE_CURRENT_SOURCE_DIR}/source/collisions
  ${CMAKE_CURRENT_SOURCE_DIR}/source/geometry
  ${CMAKE_CURRENT_SOURCE_DIR}/source/level_container
  ${CMAKE_CURRENT_SOURCE_DIR}/source/level_data
  ${CMAKE_CURRENT_SOURCE_DIR}/source/play_screen
  ${CMAKE_CURRENT_SOURCE_DIR}/source/renderers
  ${CMAKE_CURRENT_SOURCE_DIR}/source/user_interface
)
