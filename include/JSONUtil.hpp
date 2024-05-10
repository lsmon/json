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

    class Util
    {
    public:
        virtual ~Util();

        virtual std::string toJSONString() const = 0;

        static std::unique_ptr<Util> parse(const std::string &json);

        static bool validate(const std::string_view jsonString);

        static std::string_view stripCurlyBraces(std::string_view jsonView);

        static std::string_view stripSquareBrackets(std::string_view jsonView);

        /// @brief Splits a string using the character delimiter. It will return a list with the each indivual string splitted.
        /// @param strView the string to split.
        /// @param delim the delimiter character.
        /// @return list of strings splitted based on the delimiter character.
        static std::vector<std::string_view> splitStringViewBy(std::string_view strView, char delim);

        static bool isInteger(std::string_view str);

        static bool isDouble(std::string_view str);

        static bool isBoolean(std::string_view str);

        static std::string_view trim(std::string_view sv);

        static int toInteger(std::string_view value);

        static double toDouble(std::string_view value);
    };
}
#endif