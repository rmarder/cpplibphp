# cpplibphp
A single header C++ library containing useful functions based on those from the PHP standard library.

How To Use
--------
```
#include "php.hpp"

using namespace php;
```

I recommended not using the namespace as this library will collide with the global C namespace as well as the C++ std namespace in the future, if it doesn't already.
Instead, you should make your function calls in the form "result = php::function(args);"

This has no dependencies, however it does require at least a C++11 compiler or better.

(the library right now will compile under c++98 but the author expects c++11 to be the minimum required in the future)

Function List
--------

List of functions included - see the PHP documentation for how these work - this implimentation is faithful to the originals except where noted:

```
std::string ltrim(const std::string &str, const std::string &whitespace = " \n\r\t\f\v")
```
Trim characters from the start of a string.

```
std::string rtrim(const std::string &str, const std::string &whitespace = " \n\r\t\f\v")
```
Trim characters from the end of a string.

```
std::string trim(const std::string &str, const std::string &whitespace = " \n\r\t\f\v")
```
Trim characters from the start and end of a string.

```
bool str_contains(const std::string &haystack, const std::string &needle)
```
Test if string haystack contains string needle.

```
std::string strrev(std::string str)
```
Reverse a string.

```
std::string str_rot13(const std::string &str)
```
Perform ROT13 on a string.

```
std::string str_replace(const std::string &search, const std::string &replace, const std::string &subject)
```
Replace all instances of string search with string replace that are contained in subject.

```
std::string str_repeat(const std::string &str, const size_t times)
```
String repeat.

```
std::string str_pad(const std::string &input, size_t length, const std::string pad_str = " ", size_t pad_type = STR_PAD_RIGHT)
```
Pad input string to length characters using string pad_str. Use STR_PAD_RIGHT, STR_PAD_LEFT, or STR_PAD_BOTH as flags.

```
std::string ucfirst(std::string str)
```
Uppercase the first character in string str.

```
std::string lcfirst(std::string str)
```
Lowercase the first character in string str.

```
std::vector<std::string> explode(std::string const &search, std::string const &subject, int limit = INT_MAX)
```
Split string subject into a vector using string search as a delimiter.

Note: This does not handle limit like php's explode() does.

- Negative limit is undefined behavior.

- If limit is set and positive, the returned array will contain a maximum of limit elements. The last element will NOT contain the rest of string.

```
std::vector<std::string> str_split(const std::string &str, size_t length = 1)
```
Split string str into a vector with each vector element containing length characters.

```
std::string implode(const std::string &separator, const std::vector<std::string> &array)

Alternative signature:
std::string implode(const std::vector<std::string> &array)
```
Assemble vector array into a string using string separator between each vector element.

The alternative signature is equivalent to calling implode("", array);
