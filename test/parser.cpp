#include <iostream>
#include <string>

#include <catch2/catch.hpp>

#include "../src/parser.h"

using namespace std;

TEST_CASE ("C-style enum")
{
  const string code = R"(enum mybool { TRUE, FALSE };)";

  parse_context pc (code);
  const auto enums = pc.parse ();
  const auto generated_code = generate_code (enums);
  const string expected_code =
R"(const char* to_string(const mybool type) noexcept {
  switch (type) {
  case TRUE:
    return "TRUE";
  case FALSE:
    return "FALSE";
  }
  return "";
}

)";

  REQUIRE (generated_code == expected_code);
}

TEST_CASE ("C++-style enum")
{
  const string code = R"(enum class rgb { red, green, blue };)";

  parse_context pc (code);
  const auto enums = pc.parse ();
  const auto generated_code = generate_code (enums);
  const string expected_code =
R"(const char* to_string(const rgb type) noexcept {
  switch (type) {
  case rgb::red:
    return "rgb::red";
  case rgb::green:
    return "rgb::green";
  case rgb::blue:
    return "rgb::blue";
  }
  return "";
}

)";

  REQUIRE (generated_code == expected_code);
}

TEST_CASE ("Anonymous enum")
{
  const string code = R"(enum { HELLO, THERE };)";

  parse_context pc (code);
  const auto enums = pc.parse ();
  const auto generated_code = generate_code (enums);
  const string expected_code =
R"(const char* to_string(const int type) noxcept {
  switch (type) {
  case HELLO:
    return "HELLO";
  case THERE:
    return "THERE";
  }
  return "";
}

)";

  REQUIRE (generated_code == expected_code);
}

TEST_CASE ("Not an enum")
{
  const string code = "// enum";

  parse_context pc (code);
  const auto enums = pc.parse ();
  const auto generated_code = generate_code (enums);
  const string expected_code = "";

  REQUIRE (generated_code == expected_code);
}
