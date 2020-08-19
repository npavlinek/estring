#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

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

struct cpp_token {
  cpp_token_type type;
  std::string lexeme;
};

class parse_context {
 public:
  parse_context(const char* filename) : m_buffer(read_file(filename)) {}

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

  void consume_until(const std::string& str) {
    int c;
    std::size_t matched{};
    FOR_EACH_CHAR(c) {
      if (c == str[matched]) {
        ++matched;
      } else {
        matched = 0;
      }

      if (matched == str.size()) {
        for (std::size_t i = 0; i < str.size(); ++i) {
          prev_char();
        }
        break;
      }
    }
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
    FOR_EACH_CHAR(c) {
      if (!std::isalnum(c) && c != '_' && c != ':') {
        break;
      }
      buf << static_cast<char>(c);
    }
    prev_char();
    return buf.str();
  }

  std::string read_integer() {
    int c;
    std::ostringstream buf;
    FOR_EACH_CHAR(c) {
      if (!std::isdigit(c) && !std::isxdigit(c) && c != 'x') {
        break;
      }
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
        token->type = cpp_token_type::CLOSE_CURLY_BRACE;
      } else if (c == ':') {
        token->type = cpp_token_type::COLON;
      } else if (c == ',') {
        token->type = cpp_token_type::COMMA;
      } else if (c == '=') {
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
        token->type = cpp_token_type::OPEN_CURLY_BRACE;
      } else if (c == ';') {
        token->type = cpp_token_type::SEMICOLON;
      } else if (c == '\n' || c == '\r' || c == '\t' || c == ' ') {
        consume_whitespace();
        continue;
      } else {
        token->type = cpp_token_type::UNKNOWN;
      }

      return token;
    }

    token->type = cpp_token_type::EOS;
    return token;
  }

 private:
  std::stringstream m_buffer;
};

}  // namespace

int main(int argc, const char** argv) {
  if (argc < 2) {
    std::cerr << "No file provided.\n";
    return 1;
  }

  parse_context pc(argv[1]);
  pc.consume_until("enum");

  for (auto token = pc.next_token(); token->type != cpp_token_type::SEMICOLON;
       token = pc.next_token()) {
    if (token->lexeme.size() > 0) {
      std::cout << token->lexeme << '\n';
    }
  }

  std::cout << "\n----------\n\n";
  pc.consume_until("enum");

  for (auto token = pc.next_token(); token->type != cpp_token_type::SEMICOLON;
       token = pc.next_token()) {
    if (token->lexeme.size() > 0) {
      std::cout << token->lexeme << '\n';
    }
  }
}
