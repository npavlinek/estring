// SPDX-License-Identifier: Unlicense

#ifndef UTIL_H
#define UTIL_H

#include <cassert>
#include <fstream>
#include <sstream>

inline std::stringstream read_file(const char* filename) {
  std::ifstream file(filename);
  assert(file.is_open());
  std::stringstream buf;
  buf << file.rdbuf();
  return buf;
}

#endif  // UTIL_H
