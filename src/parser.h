// SPDX-License-Identifier: Unlicense

#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <sstream>
#include <string>
#include <vector>

enum class cpp_token_type
{
  close_curly_brace,
  colon,
  comma,
  end_of_stream,
  equals,
  ident,
  integer,
  open_curly_brace,
  semicolon,
  unknown
};

struct cpp_token
{
  cpp_token_type type;
  std::string lexeme;
};

struct enum_info
{
  bool scoped;
  std::string name;
  std::string type;
  std::vector<std::string> values;
};

std::string
generate_code (const std::vector<enum_info>& enums) noexcept;

class parse_context
{
public:
  parse_context () = delete;

  explicit
  parse_context (const std::string& code);

  std::vector<enum_info>
  parse ();

private:
  bool
  is_eof () const noexcept;

  int
  prev_char () noexcept;

  int
  next_char () noexcept;

  void
  consume_whitespace () noexcept;

  std::string
  read_ident () noexcept;

  std::string
  read_int () noexcept;

  std::unique_ptr<cpp_token>
  next_token ();

  std::unique_ptr<cpp_token>
  peek_token ();

  void
  discard_until (const cpp_token_type type, const std::string& lexeme);

  std::unique_ptr<cpp_token>
  expect_and_consume (const cpp_token_type type);

  std::unique_ptr<cpp_token>
  expect_and_consume (const cpp_token_type type, const std::string& lexeme);

private:
  std::stringstream buffer_;
  std::size_t last_token_size_;
};

#endif // PARSER_H
