cmake_minimum_required(VERSION 3.25)

project(buck-wood)

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

include(source/geometry/CMakeLists.txt)
foreach(FILE ${FILES})
  set(geometry ${geometry} source/geometry/${FILE})
endforeach()

include(source/level_container/CMakeLists.txt)
foreach(FILE ${FILES})
  set(level_container ${level_container} source/level_container/${FILE})
endforeach()

include(source/level_container/objects/CMakeLists.txt)
foreach(FILE ${FILES})
  set(level_objects ${level_objects} source/level_container/objects/${FILE})
endforeach()

include(source/level_data/CMakeLists.txt)
foreach(FILE ${FILES})
  set(level_data ${level_data} source/level_data/${FILE})
endforeach()

include(source/play_screen/CMakeLists.txt)
foreach(FILE ${FILES})
  set(play_screen ${play_screen} source/play_screen/${FILE})
endforeach()

include(source/renderers/CMakeLists.txt)
foreach(FILE ${FILES})
  set(renderers ${renderers} source/renderers/${FILE})
endforeach()

include(source/user_interface/CMakeLists.txt)
foreach(FILE ${FILES})
  set(user_interface ${user_interface} source/user_interface/${FILE})
endforeach()

add_executable(buck_wood ${source} ${framework} ${collisions} ${geometry} ${level_container} ${level_objects} ${level_data} ${play_screen} ${renderers} ${user_interface})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(buck_wood PRIVATE /MP /MTd /Zi /W4 /WX /fp:fast)
  target_link_options(buck_wood PRIVATE /SUBSYSTEM:WINDOWS)
endif()

target_precompile_headers(buck_wood PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source/pch.h)

target_include_directories(buck_wood PRIVATE
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
