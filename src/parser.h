// SPDX-License-Identifier: Unlicense

#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <sstream>
#include <string>
#include <vector>

enum class cpp_token_type {
  CLOSE_CURLY_BRACE,
  COLON,
  COMMA,
  EOS,
  EQUALS,
  IDENT,
  INTEGER,
  OPEN_CURLY_BRACE,
  SEMICOLON,
  UNKNOWN
};

struct cpp_token {
  cpp_token_type type;
  std::string lexeme;
};

struct enum_info {
  bool scoped;
  std::string name;
  std::string type;
  std::vector<std::string> values;
};

class parse_context {
 public:
  parse_context() = delete;
  parse_context(const std::string& code);

  std::vector<enum_info> parse();
  static std::string gen_code(const std::vector<enum_info>& enums);

 private:
  bool is_eof() const;
  int prev_char();
  int next_char();

  void consume_whitespace();
  std::string read_ident();
  std::string read_integer();

  std::unique_ptr<cpp_token> next_token();
  std::unique_ptr<cpp_token> peek_token();
  void discard_until(const cpp_token_type type, const std::string& lexeme);

  std::unique_ptr<cpp_token> expect_and_consume(const cpp_token_type type);
  std::unique_ptr<cpp_token> expect_and_consume(const cpp_token_type type,
                                                const std::string& lexeme);

  std::stringstream m_buffer;
  std::size_t m_last_token_size;
};

#endif  // PARSER_H
