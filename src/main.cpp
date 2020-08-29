// SPDX-License-Identifier: Unlicense

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "parser.h"

using namespace std;

int
main (int argc, const char** argv)
{
  if (argc < 2)
  {
    std::cerr << "No file provided.\n";
    return 1;
  }

  const auto read_file = [] (const char* filename)
  {
    ifstream file (filename);
    assert (file.is_open ());
    stringstream buf;
    buf << file.rdbuf ();
    return buf;
  };

  const auto code = read_file (argv[1]);
  parse_context pc (code.str ());
  const auto enums = pc.parse ();
  std::cout << generate_code (enums);

  return 0;
}
