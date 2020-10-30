# stringifier

## Usage

```sh
echo <input-cpp-file> | stringifier
```

or

```sh
stringifier < <input-cpp-file>
```

You can pass actual C++ source code files to `stringifier`, it'll parse all the
enums in the file and ignore the rest of C++ code.

### Example

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

a `to_string` function gets generated:

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

## Build

### Requirements

- C++11 (or later) compiler
- build2

After installing `build2`, to build just invoke `b`, like this:

```sh
b
```

Or if you want a bit more control, you can create a build configuration, for
example:

```sh
bdep init -C @gcc cc config.cxx=g++ "config.cc.coptions=-Wall -Wextra"
bdep update @gcc
```

For more information regarding build configurations specifically and `build2`
generally, refer to:

<https://www.build2.org/build2-toolchain/doc/build2-toolchain-intro.xhtml>

## Tests

`stringifier` comes with tests written in
[Testscript](https://www.build2.org/build2/doc/build2-testscript-manual.xhtml).
To run them, either use `b`, if you don't have a build configuration or `bdep`,
if you have one.

```sh
b test
```

or

```sh
bdep test
```

## Install

## License

MIT. See the `LICENSE` file for more information.
