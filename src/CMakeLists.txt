set(sources
  parser.cpp
  parser.h
)
add_library(enum_to_string_lib SHARED ${sources})

add_executable(${EXECUTABLE_NAME} main.cpp)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE enum_to_string_lib)
