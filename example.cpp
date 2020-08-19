enum class gdb_token_type : std::uint16_t {
  AMPERSAND,
  ASTERISK,
  AT,
  CARET,
  CLOSE_CURLY_BRACE,
  CLOSE_SQUARE_BRACKET,
  COMMA,
  CSTRING,
  EOS,
  EQUALS = 0x80,
  OPEN_CURLY_BRACE = 123,
  OPEN_SQUARE_BRACKET,
  PLUS,
  PROMPT,
  STRING,
  TILDE,
  TOKEN,
  UNKNOWN
};

struct something {};

enum simple { OK, NOT_OK };
