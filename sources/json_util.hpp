#ifndef JSON_UTIL_HPP
#define JSON_UTIL_HPP

#include <string>
#include <string_view>

namespace json
{
    const int OCB = 123; // OPEN CURLY BRACE '{'
    const int CCB = 125; // CLOSE CURLY BRACE '}'
    const int OSB = 91;  // OEPN SQUARE BRACKET '['
    const int CSB = 93;  // CLOSE SQUARE BRACLET ']'
    const int OP = 40;   // OPEN PARENTHESIS '('
    const int CP = 41;   // CLOSE PARENTHESIS ')'

    class util
    {
    public:
        virtual ~util();

        virtual std::string to_json_string() const = 0;

        static std::unique_ptr<util> parse(const std::string &json);

        static bool validate(const std::string_view jsonString);

        static std::string_view strip_curly_braces(std::string_view jsonView);

        static std::string_view strip_square_brackets(std::string_view jsonView);

        /// @brief Splits a string using the character delimiter. It will return a list with the each indivual string splitted.
        /// @param strView the string to split.
        /// @param delim the delimiter character.
        /// @return list of strings splitted based on the delimiter character.
        static std::vector<std::string_view> split_string_view_by(std::string_view strView, char delim);

        static bool is_integer(std::string_view str);

        static bool is_double(std::string_view str);

        static bool is_boolean(std::string_view str);

        static std::string_view trim(std::string_view sv);

        static int to_integer(std::string_view value);

        static double to_double(std::string_view value);
    };
}
#endif