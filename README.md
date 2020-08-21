# enum-to-string

## Requirements

- C++11 compiler
- C++ STL
- Catch2 for tests

## Build

```sh
cmake -B build
cmake --build build
```

## Use

```sh
./build/bin/enum-to-string <input_cpp_file>
```

## Example

Given C++ code with an enum, this:

```cpp
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
```

gets turned into:

```cpp
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
```

## License

The Unlicense. For more information see the `UNLICENSE` file.
