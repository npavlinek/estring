// file     : stringifier/enum-parser.hxx
// license  : MIT; see accompanying LICENSE file

#ifndef ENUM_PARSER_HXX
#define ENUM_PARSER_HXX

#include <memory>
#include <sstream>
#include <stdexcept>
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

class unexpected_token : public std::exception
{
public:
  unexpected_token () = delete;

  unexpected_token (const cpp_token_type expected_type,
                    const cpp_token& actual_token) noexcept;

  unexpected_token (const cpp_token_type expected_type,
                    const std::string& expected_lexeme,
                    const cpp_token& actual_token) noexcept;

  virtual const char*
  what () const noexcept override;

private:
  std::string msg_;
};

struct enum_info
{
  bool scoped;
  std::string name;
  std::string type;
  std::vector<std::string> values;

  bool
  operator== (const enum_info& other) const noexcept
  {
    return scoped == other.scoped &&
           name == other.name &&
           type == other.type &&
           values == other.values;
  }
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
  consume_comment () noexcept;

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

#endif // ENUM_PARSER_HXX
