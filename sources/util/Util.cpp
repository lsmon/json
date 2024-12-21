//
// Created by alcacruz on 5/24/24.
//

#include <charconv>
#include <algorithm>
#include <stack>
#include <regex>
#include "util/Util.hpp"
#include "JSON.hpp"
#include "util/Parser.hpp"

#include <iostream>
#include <string_view>

bool Util::validate(const std::string_view jsonString)
{
    std::stack<char> brackets;
    bool inString = false;

    for (size_t i = 0; i < jsonString.size(); ++i)
    {
        char c = jsonString[i];

        // Toggle inString flag for quotes
        if (c == '"' && (i == 0 || jsonString[i - 1] != '\\'))
        {
            inString = !inString;
            continue;
        }

        // Skip everything inside strings
        if (inString)
            continue;

        // Push opening brackets onto the stack
        if (c == '(' || c == '[' || c == '{')
        {
            brackets.push(c);
        }
        else if (c == ')' || c == ']' || c == '}') // Handle closing brackets
        {
            if (brackets.empty())
            {
                std::string what = "Error: Closing " + std::to_string(c) + " at position " + std::to_string(i) + " with an empty stack\n";
                return false;
            }

            char top = brackets.top();
            if ((c == ')' && top == '(') || (c == ']' && top == '[') || (c == '}' && top == '{'))
            {
                brackets.pop();
            }
            else
            {
                std::string what = "Error: Closing " + std::to_string(c) + " does not match opening " + std::to_string(top) + " at position " + std::to_string(i);
                throw std::runtime_error(what);
                return false;
            }
        }
    }

    // Check for unmatched opening brackets
    if (!brackets.empty())
    {
        std::cout << "Error: Unmatched opening bracket(s) left in the stack\n";
        return false;
    }

    return true;
}

Util::~Util() {}

std::unique_ptr<Util> Util::parse(const std::string &json)
{
    std::string sv = json;
    sv = Util::trim(sv);
    if (sv.front() == '{')
    {
        JSONObject obj = Parser::parseObject(sv);
        return std::make_unique<JSONObject>(obj);
    }
    else if (sv.front() == '[')
    {
        JSONArray arr = Parser::parseArray(sv);
        return std::make_unique<JSONArray>(arr);
    }
    throw std::invalid_argument("Invalid JSON string.");
}

std::string_view Util::stripCurlyBraces(std::string_view jsonView)
{
    if (!jsonView.empty() && jsonView.front() == OCB && jsonView.back() == CCB)
    {
        return jsonView.substr(1, jsonView.length() - 2);
    }
    else
    {
        throw std::runtime_error("String does not start and end with curly braces");
    }
}

std::string_view Util::stripSquareBrackets(std::string_view jsonView)
{
    if (!jsonView.empty() && jsonView.front() == '[' && jsonView.back() == ']')
    {
        return jsonView.substr(1, jsonView.size() - 2);
    }
    else
    {
        throw std::runtime_error("String does not start and end with square braces");
    }
}

std::vector<std::string_view> Util::splitStringViewBy(std::string_view strView, char delim)
{
    std::vector<std::string_view> result;
    size_t start = 0;
    int openBrackets = 0;
    bool inQuotes = false;

    for (size_t i = 0; i < strView.size(); ++i) {
        if (strView[i] == '[' || strView[i] == '{') {
            openBrackets++;
        } else if (strView[i] == ']' || strView[i] == '}') {
            openBrackets--;
        } else if (strView[i] == '\"') {
            inQuotes = !inQuotes;
        }

        if (strView[i] == delim && openBrackets == 0 && !inQuotes) {
            result.push_back(strView.substr(start, i - start));
            start = i + 1;
        }
    }
    result.push_back(strView.substr(start));

    return result;
}

bool Util::isInteger(std::string_view str)
{
    std::regex pattern("^-?\\d+$");

    if (std::regex_match(str.begin(), str.end(), pattern) && str.size() <= 10) {
        return true;
    } else {
        return false;
    }
}

bool Util::isLong(std::string_view str)
{
    std::regex pattern("^-?\\d+$");

    if (std::regex_match(str.begin(), str.end(), pattern) && str.size() <= 10) {
        return true;
    } else {
        return false;
    }
}

bool Util::isLongLong(std::string_view str) {
    std::regex pattern("^-?\\d+$");

    if (std::regex_match(str.begin(), str.end(), pattern) && str.size() <= 20) {
        return true;
    } else {
        return false;
    }
}

bool Util::isDouble(std::string_view str)
{
    if (str.empty())
        return false;

    size_t startPos = 0;
    if (str[0] == '+' || str[0] == '-')
        startPos = 1;

    bool dotSeen = false;

    for (size_t i = startPos; i < str.size(); ++i)
    {
        if (str[i] == '.')
        {
            if (dotSeen || i == startPos || i == str.size() - 1)
                return false;
            dotSeen = true;
        }
        else if (!std::isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool Util::isBoolean(std::string_view str)
{
    return str == "true" || str == "false" || str == "True" || str == "False";
}

std::string_view Util::trim(std::string_view stringView)
{
    while (!stringView.empty() && std::isspace(stringView.front()))
    {
        stringView.remove_prefix(1);
    }
    while (!stringView.empty() && std::isspace(stringView.back()))
    {
        stringView.remove_suffix(1);
    }
    return stringView;
}

std::string_view Util::trimQuotes(std::string_view str) {
    size_t start = 0;
    size_t end = str.length();

    // Check if the first character is a double quote
    if (start < end && str[start] == '"') {
        ++start;
    }

    // Check if the last character is a double quote
    if (start < end && str[end - 1] == '"') {
        --end;
    }

    // Return the trimmed substring
    return str.substr(start, end - start);
}

std::string sanitizeNumericString(std::string_view value) {
    std::string sanitized;
    for (char ch : value) {
        if (std::isdigit(ch) || ch == '-') { // Include '-' for negative numbers
            sanitized += ch;
        }
    }
    return sanitized;
}

int Util::toInteger(std::string_view value)
{
    std::string sanitized = sanitizeNumericString(value);
    int intValue;
    auto [ptr, ec] = std::from_chars(sanitized.data(), sanitized.data() + sanitized.size(), intValue);
    if (ec != std::errc())
    {
        std::cerr << "Failed to convert: " << value << std::endl;
        throw std::runtime_error("Conversion to int failed");
    }
    return intValue;
}

long Util::toLong(std::string_view value)
{
    std::string sanitized = sanitizeNumericString(value);
    long longValue;
    auto [ptr, ec] = std::from_chars(sanitized.data(), sanitized.data() + sanitized.size(), longValue);
    if (ec != std::errc()) {
        std::cerr << "Failed to convert: " << value << std::endl;
        throw std::runtime_error("Conversion to long failed");
    }
    return longValue;
}

long long Util::toLongLong(std::string_view value) {
    long long longValue;
    auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), longValue);
    if (ec != std::errc()) {
        std::cerr << "Failed to convert: " << value << std::endl;
        throw std::runtime_error("Conversion to long long failed");
    }
    return longValue;
}

double Util::toDouble(std::string_view value)
{
    try
    {
        std::string str(value);
        return std::stod(str);
    }
    catch (const std::invalid_argument &ia)
    {
        throw std::runtime_error("Invalid argument");
    }
    catch (const std::out_of_range &)
    {
        throw std::out_of_range("Out of range");
    }
}
