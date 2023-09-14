add_library(src "")

target_sources(src
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/scheduler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/stat.cpp
    ${CMAKE_CURRENT_LIST_DIR}/task.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/scheduler.h
    ${CMAKE_CURRENT_LIST_DIR}/stat.h
    ${CMAKE_CURRENT_LIST_DIR}/task.h
  )
target_include_directories(src
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )