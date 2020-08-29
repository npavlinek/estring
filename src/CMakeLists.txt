set(sources
  parser.cpp
  parser.h)
add_library(enum_to_string_lib SHARED ${sources})

add_executable(${PNAME} main.cpp)
target_link_libraries(${PNAME} PRIVATE enum_to_string_lib)
