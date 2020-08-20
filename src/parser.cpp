#include <cctype>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "parser.h"

#define FOR_EACH_CHAR(var) \
  for (var = next_char(); var != EOF; var = next_char())

parse_context::parse_context(const char* filename)
    : m_buffer(read_file(filename)), m_last_token_size{} {}

bool parse_context::is_eof() const {
  return m_buffer.eof();
}

int parse_context::prev_char() {
  if (m_buffer.tellg() != 0) {
    m_buffer.unget();
  }
  return m_buffer.peek();
}

int parse_context::next_char() {
  return is_eof() ? EOF : m_buffer.get();
}

void parse_context::consume_whitespace() {
  int c;
  FOR_EACH_CHAR(c) {
    if (!std::isspace(c)) {
      break;
    }
  }
  prev_char();
}

std::string parse_context::read_ident() {
  int c;
  std::ostringstream buf;
  m_last_token_size = 0;
  FOR_EACH_CHAR(c) {
    if (!std::isalnum(c) && c != '_' && c != ':') {
      break;
    }
    ++m_last_token_size;
    buf << static_cast<char>(c);
  }
  prev_char();
  return buf.str();
}

std::string parse_context::read_integer() {
  int c;
  std::ostringstream buf;
  m_last_token_size = 0;
  FOR_EACH_CHAR(c) {
    if (!std::isdigit(c) && !std::isxdigit(c) && c != 'x') {
      break;
    }
    ++m_last_token_size;
    buf << static_cast<char>(c);
  }
  prev_char();
  return buf.str();
}

std::unique_ptr<cpp_token> parse_context::next_token() {
  int c;
  std::unique_ptr<cpp_token> token(new cpp_token);

  FOR_EACH_CHAR(c) {
    if (is_eof()) {
      break;
    }

    if (c == '}') {
      m_last_token_size = 1;
      token->type = cpp_token_type::CLOSE_CURLY_BRACE;
    } else if (c == ':') {
      m_last_token_size = 1;
      token->type = cpp_token_type::COLON;
    } else if (c == ',') {
      m_last_token_size = 1;
      token->type = cpp_token_type::COMMA;
    } else if (c == '=') {
      m_last_token_size = 1;
      token->type = cpp_token_type::EQUALS;
    } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_')) {
      prev_char();
      token->type = cpp_token_type::IDENT;
      token->lexeme = read_ident();
    } else if (c >= '0' && c <= '9') {
      prev_char();
      token->type = cpp_token_type::INTEGER;
      token->lexeme = read_integer();
    } else if (c == '{') {
      m_last_token_size = 1;
      token->type = cpp_token_type::OPEN_CURLY_BRACE;
    } else if (c == ';') {
      m_last_token_size = 1;
      token->type = cpp_token_type::SEMICOLON;
    } else if (c == '\n' || c == '\r' || c == '\t' || c == ' ') {
      consume_whitespace();
      continue;
    } else {
      m_last_token_size = 1;
      token->type = cpp_token_type::UNKNOWN;
    }

    return token;
  }

  token->type = cpp_token_type::EOS;
  return token;
}

std::unique_ptr<cpp_token> parse_context::peek_token() {
  auto token = next_token();
  while (m_last_token_size > 0) {
    --m_last_token_size;
    prev_char();
  }
  return token;
}

void parse_context::discard_until(const cpp_token_type type,
                                  const std::string& lexeme) {
  for (auto token = peek_token(); token->type != cpp_token_type::EOS;
       token = peek_token()) {
    if (token->type == type && token->lexeme == lexeme) {
      break;
    }
    next_token();
  }
}

std::unique_ptr<cpp_token> parse_context::expect_and_consume(
    const cpp_token_type type) {
  auto token = next_token();
  if (token->type != type) {
    throw std::runtime_error("Unexpected token.");
  }
  return token;
}

std::unique_ptr<cpp_token> parse_context::expect_and_consume(
    const cpp_token_type type, const std::string& lexeme) {
  auto token = next_token();
  if (token->type != type || token->lexeme != lexeme) {
    throw std::runtime_error("Unexpected token.");
  }
  return token;
}
