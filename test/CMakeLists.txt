find_package(Catch2 CONFIG REQUIRED)

set(test_sources
  main.cpp
  parser.cpp
)

add_executable(run-tests ${test_sources})
target_link_libraries(run-tests PRIVATE Catch2::Catch2)
target_link_libraries(run-tests PRIVATE enum_to_string_lib)
