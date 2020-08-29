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
enum-to-string <input_cpp_file>
```

## Example

Given a C++ enum like this:

```cpp
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
```

a `to_string` function like this gets generated:

```cpp
const char* to_string(const cpp_token_type type) noexcept {
  switch (type) {
  case cpp_token_type::close_curly_brace:
    return "cpp_token_type::close_curly_brace";
  case cpp_token_type::colon:
    return "cpp_token_type::colon";
  case cpp_token_type::comma:
    return "cpp_token_type::comma";
  case cpp_token_type::eos:
    return "cpp_token_type::eos";
  case cpp_token_type::equals:
    return "cpp_token_type::equals";
  case cpp_token_type::ident:
    return "cpp_token_type::ident";
  case cpp_token_type::integer:
    return "cpp_token_type::integer";
  case cpp_token_type::open_curly_brace:
    return "cpp_token_type::open_curly_brace";
  case cpp_token_type::semicolon:
    return "cpp_token_type::semicolon";
  case cpp_token_type::unknown:
    return "cpp_token_type::unknown";
  }
  return "";
}
```

## License

The Unlicense. For more information see the `UNLICENSE` file.
