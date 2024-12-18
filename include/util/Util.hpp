//
// Created by alcacruz on 5/24/24.
//

#ifndef JSONPP_UTIL_HPP
#define JSONPP_UTIL_HPP

#include <string>
#include <memory>
#include <vector>

const int OCB = 123; // OPEN CURLY BRACE '{'
const int CCB = 125; // CLOSE CURLY BRACE '}'
const int OSB = 91;  // OEPN SQUARE BRACKET '['
const int CSB = 93;  // CLOSE SQUARE BRACLET ']'
const int OP = 40;   // OPEN PARENTHESIS '('
const int CP = 41;   // CLOSE PARENTHESIS ')'

class Util {
public:

    virtual ~Util();

    virtual std::string str() const = 0;

    virtual std::string dump(const int &indentSz = 0, const int &currentSz = 0) const = 0;

    static std::unique_ptr<Util> parse(const std::string &json);

    static std::string_view stripCurlyBraces(std::string_view jsonView);

    static std::string_view stripSquareBrackets(std::string_view jsonView);

    static std::vector<std::string_view> splitStringViewBy(std::string_view strView, char delim);

    static bool isInteger(std::string_view str);

    static bool isLong(std::string_view str);

    static bool isDouble(std::string_view str);

    static bool isBoolean(std::string_view str);

    static std::string_view trim(std::string_view sv);

    static std::string_view trimQuotes(std::string_view str);

    static int toInteger(std::string_view value);

    static double toDouble(std::string_view value);

    static long toLong(std::string_view str);

    static bool validate(std::string_view jsonString);
};


#endif //JSONPP_UTIL_HPP
