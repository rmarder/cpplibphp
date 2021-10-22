# cpplibphp
A single header C++ library containing useful functions based on those from the PHP standard library.

List of functions included - see the PHP documentation for how these work - this implimentation is faithful to the originals except where noted:

std::string ltrim(const std::string &str, const std::string &whitespace = " \n\r\t\f\v")

std::string rtrim(const std::string &str, const std::string &whitespace = " \n\r\t\f\v")

std::string trim(const std::string &str, const std::string &whitespace = " \n\r\t\f\v")

bool str_contains(const std::string &haystack, const std::string &needle)

std::string strrev(std::string str)

std::string str_rot13(const std::string &str)

std::string str_replace(const std::string &search, const std::string &replace, const std::string &subject)

std::string str_repeat(const std::string &str, const size_t times)

std::string str_pad(const std::string &input, size_t length, const std::string pad_str = " ", size_t pad_type = STR_PAD_RIGHT)

std::string ucfirst(std::string str)

std::string lcfirst(std::string str)

std::vector<std::string> explode(std::string const &search, std::string const &subject, int limit = INT_MAX)
// This does not handle limit like php's explode() does.
// 1) Negative limit is undefined behavior.
// 2) If limit is set and positive, the returned array will contain a maximum of limit elements. The last element will NOT contain the rest of string.

std::vector<std::string> str_split(const std::string &str, size_t length = 1)

std::string implode(const std::string &separator, const std::vector<std::string> &array)
std::string implode(const std::vector<std::string> &array)
