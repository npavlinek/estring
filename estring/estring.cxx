// file     : estring/estring.cxx
// license  : MIT; see accompanying LICENSE file

#include <iostream>
#include <sstream>

#include <estring/enum-parser.hxx>

using namespace std;

int
main ()
{
  ostringstream code;
  for (string s; getline (cin, s);)
    code << s << '\n';

  try
  {
    parse_context pc (code.str ());
    const auto enums = pc.parse ();
    cout << generate_code (enums);
  }
  catch (const unexpected_token& e)
  {
    cerr << "ERROR: " << e.what ();
    return 1;
  }

  return 0;
}
