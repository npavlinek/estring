// SPDX-License-Identifier: Unlicense

#include <iostream>
#include <vector>

#include "parser.h"

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

    enum_info einfo{};
    token = pc.expect_and_consume(cpp_token_type::IDENT, "enum");

    token = pc.peek_token();
    if (token->lexeme == "class" || token->lexeme == "struct") {
      pc.next_token();
      einfo.scoped = true;
    } else {
      einfo.scoped = false;
    }

    // This is a bit of a hack. Because of the way this loop works, it treats
    // any 'enum' it encounters as a keyword, this is a problem if that 'enum'
    // is not actually a keyword (e.g. 'enum' appears in a comment).
    //
    // So here we check if the token after that 'enum' is an IDENT and we read
    // the enum name if that's the case, otherwise we ignore it.
    token = pc.peek_token();
    if (token->type == cpp_token_type::IDENT) {
      token = pc.next_token();
      einfo.name = std::move(token->lexeme);
    }

    token = pc.peek_token();
    if (token->type == cpp_token_type::COLON) {
      pc.next_token();
      token = pc.expect_and_consume(cpp_token_type::IDENT);
      einfo.type = std::move(token->lexeme);
    }

    token = pc.peek_token();
    if (token->type == cpp_token_type::SEMICOLON ||
        token->type != cpp_token_type::OPEN_CURLY_BRACE) {
      pc.next_token();
      continue;
    }
    pc.expect_and_consume(cpp_token_type::OPEN_CURLY_BRACE);

    token = pc.peek_token();
    while (token->type != cpp_token_type::CLOSE_CURLY_BRACE) {
      token = pc.peek_token();
      if (token->type != cpp_token_type::IDENT) {
        break;
      }
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
    if (e.name.size() > 0) {
      std::cout << "const char* to_string(const " << e.name << " type) {\n"
                << "  switch (type) {\n";
    } else {
      std::cout << "const char* to_string(const int type) {\n"
                << "  switch (type) {\n";
    }

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
