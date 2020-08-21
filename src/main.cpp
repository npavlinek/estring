// SPDX-License-Identifier: Unlicense

#include <iostream>

#include "parser.h"
#include "util.h"

int main(int argc, const char** argv) {
  if (argc < 2) {
    std::cerr << "No file provided.\n";
    return 1;
  }

  const auto code = read_file(argv[1]);
  parse_context pc(code.str());
  const auto enums = pc.parse();
  std::cout << parse_context::gen_code(enums);

  return 0;
}
