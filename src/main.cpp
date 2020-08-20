#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#define FOR_EACH_CHAR(var) \
  for (var = next_char(); var != EOF; var = next_char())

namespace {

std::stringstream read_file(const char* filename) {
  std::ifstream file(filename);
  assert(file.is_open());
  std::stringstream buf;
  buf << file.rdbuf();
  return buf;
}

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

const char* to_string(const cpp_token_type type) {
  switch (type) {
    case cpp_token_type::CLOSE_CURLY_BRACE:
      return "cpp_token_type::CLOSE_CURLY_BRACE";
    case cpp_token_type::COLON:
      return "cpp_token_type::COLON";
    case cpp_token_type::COMMA:
      return "cpp_token_type::COMMA";
    case cpp_token_type::EOS:
      return "cpp_token_type::EOS";
    case cpp_token_type::EQUALS:
      return "cpp_token_type::EQUALS";
    case cpp_token_type::IDENT:
      return "cpp_token_type::IDENT";
    case cpp_token_type::INTEGER:
      return "cpp_token_type::INTEGER";
    case cpp_token_type::OPEN_CURLY_BRACE:
      return "cpp_token_type::OPEN_CURLY_BRACE";
    case cpp_token_type::SEMICOLON:
      return "cpp_token_type::SEMICOLON";
    case cpp_token_type::UNKNOWN:
      return "cpp_token_type::UNKNOWN";
  }
  return "";
}

struct cpp_token {
  cpp_token_type type;
  std::string lexeme;
};

class parse_context {
 public:
  parse_context() = delete;
  parse_context(const char* filename)
      : m_buffer(read_file(filename)), m_last_token_size{} {}

  bool is_eof() const {
    return m_buffer.eof();
  }

  int prev_char() {
    if (m_buffer.tellg() != 0) {
      m_buffer.unget();
    }
    return m_buffer.peek();
  }

  int next_char() {
    return is_eof() ? EOF : m_buffer.get();
  }

  void consume_whitespace() {
    int c;
    FOR_EACH_CHAR(c) {
      if (!std::isspace(c)) {
        break;
      }
    }
    prev_char();
  }

  std::string read_ident() {
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

  std::string read_integer() {
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

  std::unique_ptr<cpp_token> next_token() {
    int c;
    auto token = std::make_unique<cpp_token>();

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
      } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
                 (c == '_')) {
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
        m_last_token_size = 0;
        token->type = cpp_token_type::UNKNOWN;
      }

      std::cout << "Type: " << to_string(token->type) << '\n';
      return token;
    }

    token->type = cpp_token_type::EOS;
    return token;
  }

  std::unique_ptr<cpp_token> peek_token() {
    auto token = next_token();
    while (m_last_token_size > 0) {
      --m_last_token_size;
      prev_char();
    }
    return token;
  }

  void discard_until(const cpp_token_type type, const std::string& lexeme) {
    for (auto token = peek_token(); token->type != cpp_token_type::EOS;
         token = peek_token()) {
      if (token->type == cpp_token_type::IDENT && token->lexeme == lexeme) {
        break;
      }
      next_token();
    }
  }

  std::unique_ptr<cpp_token> expect_and_consume(const cpp_token_type type) {
    auto token = next_token();
    if (token->type != type) {
      std::cerr << "Expected " << to_string(type) << ", got "
                << to_string(token->type) << '\n';
      throw std::runtime_error("Unexpected token.");
    }
    return token;
  }

  std::unique_ptr<cpp_token> expect_and_consume(const cpp_token_type type,
                                                const std::string& lexeme) {
    auto token = next_token();
    if (token->type != type || token->lexeme != lexeme) {
      std::cerr << "Expected " << to_string(type) << ", got "
                << to_string(token->type) << '\n';
      throw std::runtime_error("Unexpected token.");
    }
    return token;
  }

 private:
  std::stringstream m_buffer;
  std::size_t m_last_token_size;
};

struct enum_info {
  bool scoped;
  std::string name;
  std::string type;
  std::unordered_map<std::string, std::string> values;
};

}  // namespace

int main(int argc, const char** argv) {
  if (argc < 2) {
    std::cerr << "No file provided.\n";
    return 1;
  }

  std::vector<enum_info> enums;
  parse_context pc(argv[1]);

  for (;;) {
    pc.discard_until(cpp_token_type::IDENT, "enum");

    auto token = pc.peek_token();
    if (token->type == cpp_token_type::EOS) {
      break;
    }

    enum_info einfo;
    token = pc.expect_and_consume(cpp_token_type::IDENT, "enum");

    token = pc.peek_token();
    if (token->lexeme == "class" || token->lexeme == "struct") {
      pc.next_token();
      einfo.scoped = true;
    } else {
      einfo.scoped = false;
    }

    token = pc.expect_and_consume(cpp_token_type::IDENT);
    einfo.name = std::move(token->lexeme);

    token = pc.peek_token();
    if (token->type == cpp_token_type::COLON) {
      pc.next_token();
      token = pc.expect_and_consume(cpp_token_type::IDENT);
      einfo.type = std::move(token->lexeme);
    }

    pc.expect_and_consume(cpp_token_type::OPEN_CURLY_BRACE);

    token = pc.peek_token();
    while (token->type != cpp_token_type::CLOSE_CURLY_BRACE) {
      token = pc.expect_and_consume(cpp_token_type::IDENT);
      const auto val_name = std::move(token->lexeme);
      einfo.values[val_name];

      token = pc.peek_token();
      if (token->type == cpp_token_type::EQUALS) {
        pc.next_token();
        token = pc.expect_and_consume(cpp_token_type::INTEGER);
        einfo.values[val_name] = token->lexeme;
      }

      token = pc.peek_token();
      if (token->type == cpp_token_type::COMMA) {
        pc.next_token();
      }
    }

    token = pc.expect_and_consume(cpp_token_type::CLOSE_CURLY_BRACE);
    token = pc.expect_and_consume(cpp_token_type::SEMICOLON);

    enums.emplace_back(einfo);
  }

  for (const auto& e : enums) {
    std::cout << "const char* to_string(const " << e.name << " type) {\n"
              << "  switch (type) {\n";

    for (const auto& val : e.values) {
      if (e.scoped) {
        std::cout << "    case " << e.name << "::" << val.first << ":\n"
                  << "      return \"" << e.name << "::" << val.first
                  << "\";\n";
      } else {
        std::cout << "    case " << val.first << ":\n"
                  << "      return \"" << val.first << "\";\n";
      }
    }

    std::cout << "  }\n"
              << "  return \"\";\n"
              << "}\n\n";
  }

  return 0;
}
