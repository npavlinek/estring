#include <iostream>
#include <string>

#include <catch2/catch.hpp>

#include "../src/parser.h"

TEST_CASE("C-style enum") {
  const std::string code = R"(
enum mybool { TRUE, FALSE };
  )";

  parse_context pc(code);
  const auto enums = pc.parse();
  const auto gen_code = parse_context::gen_code(enums);
  const std::string expected_code =
      R"(const char* to_string(const mybool type) {
  switch (type) {
    case TRUE:
      return "TRUE";
    case FALSE:
      return "FALSE";
  }
  return "";
}

)";

  REQUIRE(gen_code == expected_code);
}

TEST_CASE("C++-style enum") {
  const std::string code = R"(
enum class rgb { RED, GREEN, BLUE };
  )";

  parse_context pc(code);
  const auto enums = pc.parse();
  const auto gen_code = parse_context::gen_code(enums);
  const std::string expected_code =
      R"(const char* to_string(const rgb type) {
  switch (type) {
    case rgb::RED:
      return "rgb::RED";
    case rgb::GREEN:
      return "rgb::GREEN";
    case rgb::BLUE:
      return "rgb::BLUE";
  }
  return "";
}

)";

  REQUIRE(gen_code == expected_code);
}

TEST_CASE("Anonymous enum") {
  const std::string code = R"(
enum { HELLO, THERE };
  )";

  parse_context pc(code);
  const auto enums = pc.parse();
  const auto gen_code = parse_context::gen_code(enums);
  const std::string expected_code = R"(const char* to_string(const int type) {
  switch (type) {
    case HELLO:
      return "HELLO";
    case THERE:
      return "THERE";
  }
  return "";
}

)";

  REQUIRE(gen_code == expected_code);
}

TEST_CASE("Not an enum") {
  const std::string code = R"(
// enum
  )";

  parse_context pc(code);
  const auto enums = pc.parse();
  const auto gen_code = parse_context::gen_code(enums);
  const std::string expected_code = "";

  REQUIRE(gen_code == expected_code);
}
