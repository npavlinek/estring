cmake_minimum_required(VERSION 3.9)

set(PNAME enum_to_string)
string(REPLACE "_" "-" EXECUTABLE_NAME ${PNAME})

project(
  ${PNAME}
  VERSION 0.0.1
  DESCRIPTION ""
  LANGUAGES CXX
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# On Windows when building a shared library, if this isn't specified, MSVC
# won't be able to link properly since there's no .lib file accompanying the
# .dll file. Static linking works just fine.
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

option(BUILD_TESTS "Build tests")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(compiler_flags
    -Weverything
    -Wfatal-errors
    -Wno-c++98-compat
    -Wno-documentation
    -Wno-padded
    -Wpedantic
    -fansi-escape-codes
    -fcolor-diagnostics)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(compiler_flags
    -Walloc-zero
    -Walloca
    -Wattribute-alias
    -Wcast-align=strict
    -Wcast-qual
    -Wconversion
    -Wdouble-promotion
    -Wduplicated-branches
    -Wduplicated-cond
    -Wfatal-errors
    -Wfloat-equal
    -Wformat-nonliteral
    -Wformat-overflow=2
    -Wformat-security
    -Wformat-signedness
    -Wformat-truncation=2
    -Wformat-y2k
    -Wformat=2
    -Wimplicit-fallthrough
    -Winit-self
    -Wlogical-op
    -Wmissing-declarations
    -Wmissing-include-dirs
    -Wnull-dereference
    -Wpacked
    -Wredundant-decls
    -Wshadow
    -Wshift-negative-value
    -Wshift-overflow=2
    -Wswitch-enum
    -Wtrampolines
    -Wundef
    -Wuninitialized
    -Wunknown-pragmas
    -Wunused
    -Wunused-macros
    -Wvla
    -Wall
    -Wextra
    -Wpedantic
    -fdiagnostics-color=always)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(compiler_flags /W4)
endif()

add_subdirectory(src)
target_compile_options(${EXECUTABLE_NAME} PRIVATE ${compiler_flags})

if(BUILD_TESTS)
  add_subdirectory(test)
  target_compile_options(run-tests PRIVATE ${compiler_flags})
endif()
