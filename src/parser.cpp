// SPDX-License-Identifier: Unlicense

#include "parser.h"

#include <cassert>
#include <cctype>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>

#define FOR_EACH_CHAR(var) \
  for (var = next_char (); var != EOF; var = next_char ())

using namespace std;

string
generate_code (const vector<enum_info>& enums) noexcept
{
  ostringstream buf;

#define INDENT(size) string(size, ' ')

  for (const auto& e : enums)
  {
    if (e.name.size () > 0)
      buf << "const char* to_string(const " << e.name << " type) noexcept {\n"
          << INDENT (2) << "switch (type) {\n";
    else
      buf << "const char* to_string(const int type) {\n"
          << INDENT (2) << "switch (type) {\n";

    for (const auto& val : e.values)
    {
      if (e.scoped)
        buf << INDENT (2) << "case " << e.name << "::" << val << ":\n"
            << INDENT (4) << "return \"" << e.name << "::" << val << "\";\n";
      else
        buf << INDENT (2) << "case " << val << ":\n"
            << INDENT (4) << "return \"" << val << "\";\n";
    }

    buf << INDENT (2) << "}\n"
        << INDENT (2) << "return \"\";\n}\n\n";
  }

#undef INDENT

  return buf.str ();
}

parse_context::
parse_context (const string& code)
  : buffer_ (code), last_token_size_ {}
{
}

vector<enum_info> parse_context::
parse ()
{
  vector<enum_info> enums;

  for (;;)
  {
    discard_until (cpp_token_type::ident, "enum");

    auto token = peek_token ();
    if (token->type == cpp_token_type::end_of_stream)
      break;

    enum_info einfo{};
    token = expect_and_consume (cpp_token_type::ident, "enum");

    token = peek_token ();
    if (token->lexeme == "class" || token->lexeme == "struct")
    {
      next_token ();
      einfo.scoped = true;
    }
    else
      einfo.scoped = false;

    token = peek_token ();
    if (token->type == cpp_token_type::ident)
    {
      token = next_token ();
      einfo.name = move (token->lexeme);
    }

    token = peek_token ();
    if (token->type == cpp_token_type::colon)
    {
      next_token ();
      token = expect_and_consume (cpp_token_type::ident);
      einfo.type = move (token->lexeme);
    }

    token = peek_token ();
    if (token->type == cpp_token_type::semicolon ||
        token->type != cpp_token_type::open_curly_brace)
    {
      next_token ();
      continue;
    }
    expect_and_consume (cpp_token_type::open_curly_brace);

    token = peek_token ();
    while (token->type != cpp_token_type::close_curly_brace)
    {
      token = peek_token ();
      if (token->type != cpp_token_type::ident)
        break;
      token = expect_and_consume (cpp_token_type::ident);
      einfo.values.emplace_back (move (token->lexeme));

      token = peek_token ();
      if (token->type == cpp_token_type::equals)
      {
        for (auto t = peek_token ();
             t->type != cpp_token_type::comma &&
             t->type != cpp_token_type::close_curly_brace;
             t = peek_token ())
          next_token ();
      }

      token = peek_token ();
      if (token->type == cpp_token_type::comma)
        next_token ();
    }

    token = expect_and_consume (cpp_token_type::close_curly_brace);
    token = expect_and_consume (cpp_token_type::semicolon);

    enums.emplace_back (einfo);
  }

  return enums;
}

bool parse_context::
is_eof () const noexcept
{
  return buffer_.eof ();
}

int parse_context::
prev_char () noexcept
{
  if (buffer_.tellg () != 0)
    buffer_.unget ();
  return buffer_.peek ();
}

int parse_context::
next_char () noexcept
{
  return is_eof () ? EOF : buffer_.get ();
}

void parse_context::
consume_whitespace () noexcept
{
  int c;
  FOR_EACH_CHAR (c)
  {
    if (!isspace (c))
      break;
  }
  prev_char ();
}

string parse_context::
read_ident () noexcept
{
  int c;
  ostringstream buf;
  last_token_size_ = 0;
  FOR_EACH_CHAR (c)
  {
    if (!isalnum (c) && c != '_' && c != ':')
      break;
    ++last_token_size_;
    buf << static_cast<char> (c);
  }
  prev_char ();
  return buf.str ();
}

string parse_context::
read_int () noexcept
{
  int c;
  ostringstream buf;
  last_token_size_ = 0;
  FOR_EACH_CHAR (c)
  {
    if (!isdigit (c) && !isxdigit (c) && c != 'x')
      break;
    ++last_token_size_;
    buf << static_cast<char> (c);
  }
  prev_char ();
  return buf.str ();
}

unique_ptr<cpp_token> parse_context::
next_token ()
{
  int c;
  unique_ptr<cpp_token> token (new cpp_token);

  FOR_EACH_CHAR (c)
  {
    if (is_eof ())
      break;

    if (c == '}')
    {
      last_token_size_ = 1;
      token->type = cpp_token_type::close_curly_brace;
    }
    else if (c == ':')
    {
      last_token_size_ = 1;
      token->type = cpp_token_type::colon;
    }
    else if (c == ',')
    {
      last_token_size_ = 1;
      token->type = cpp_token_type::comma;
    }
    else if (c == '=')
    {
      last_token_size_ = 1;
      token->type = cpp_token_type::equals;
    }
    else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_'))
    {
      prev_char ();
      token->type = cpp_token_type::ident;
      token->lexeme = read_ident ();
    }
    else if (c >= '0' && c <= '9')
    {
      prev_char ();
      token->type = cpp_token_type::integer;
      token->lexeme = read_int ();
    }
    else if (c == '{')
    {
      last_token_size_ = 1;
      token->type = cpp_token_type::open_curly_brace;
    }
    else if (c == ';')
    {
      last_token_size_ = 1;
      token->type = cpp_token_type::semicolon;
    }
    else if (c == '\n' || c == '\r' || c == '\t' || c == ' ')
    {
      consume_whitespace ();
      continue;
    }
    else
    {
      last_token_size_ = 1;
      token->type = cpp_token_type::unknown;
    }

    return token;
  }

  token->type = cpp_token_type::end_of_stream;
  return token;
}

unique_ptr<cpp_token> parse_context::
peek_token ()
{
  auto token = next_token ();
  while (last_token_size_ > 0)
  {
    --last_token_size_;
    prev_char ();
  }
  return token;
}

void parse_context::
discard_until (const cpp_token_type type, const string& lexeme)
{
  for (auto token = peek_token ();
       token->type != cpp_token_type::end_of_stream;
       token = peek_token ())
  {
    if (token->type == type && token->lexeme == lexeme)
      break;
    next_token ();
  }
}

unique_ptr<cpp_token> parse_context::
expect_and_consume (const cpp_token_type type)
{
  auto token = next_token ();
  if (token->type != type)
    throw runtime_error ("Unexpected token.");
  return token;
}

unique_ptr<cpp_token> parse_context::
expect_and_consume (const cpp_token_type type, const string& lexeme)
{
  auto token = next_token ();
  if (token->type != type || token->lexeme != lexeme)
    throw runtime_error ("Unexpected token.");
  return token;
}
