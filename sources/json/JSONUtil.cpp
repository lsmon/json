#include "json/JSONUtil.hpp"
#include <string_view>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <charconv>
#include <stack>
#include <memory>
#include "json/JSONObject.hpp"
#include "json/JSONArray.hpp"

bool json::Util::validate(const std::string_view jsonString)
{
    std::stack<char> brackets;
    for (auto c : jsonString)
    {
        switch (c)
        {
        case '(':
        case '[':
        case '{':
            brackets.push(c);
            break;
        case ')':
            if (brackets.top() == '(')
                brackets.pop();
            else
                return false;
            break;
        case ']':
            if (brackets.top() == '[')
                brackets.pop();
            else
                return false;
            break;
        case '}':
            if (brackets.top() == '{')
                brackets.pop();
            else
                return false;
            break;
        default:
            break;
        }
    }
    return brackets.empty();
}

json::Util::~Util()
{
}

std::unique_ptr<json::Util> json::Util::parse(const std::string &json)
{
    std::string_view sv = std::string_view(json);
    sv = json::Util::trim(sv);
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

std::string_view json::Util::stripCurlyBraces(std::string_view jsonView)
{
    if (!jsonView.empty() && jsonView.front() == OCB && jsonView.back() == CCB)
    {
        jsonView = jsonView.substr(1, jsonView.length() - 2);
    }
    else
    {
        throw std::runtime_error("String does not start and end with curly braces");
    }
    return jsonView;
}

std::string_view json::Util::stripSquareBrackets(std::string_view jsonView)
{
    if (!jsonView.empty() && jsonView.front() == OSB && jsonView.back() == CSB)
    {
        jsonView = jsonView.substr(1, jsonView.length() - 2);
    }
    else
    {
        throw std::runtime_error("String does not start and end with square brackets");
    }
    return jsonView;
}

std::vector<std::string_view> json::Util::splitStringViewBy(std::string_view strView, char delim)
{
    std::vector<std::string_view> result;
    size_t start = 0;
    size_t end = 0;

    while ((end = strView.find(delim, start)) != std::string_view::npos)
    {
        std::string_view token;
        int openBraces = strView.find(OCB, start);
        int openBrackets = strView.find(OSB, start);
        if (openBraces > 0 || openBrackets > 0)
        {
            if (openBraces < end)
            {
                int closeBraces = strView.find(CCB, start);
                if (openBraces < closeBraces && closeBraces > end)
                    end = closeBraces + 1;
            }

            if (openBrackets < end)
            {
                int closeBrackets = strView.find(CSB, start);
                if (openBrackets < closeBrackets && closeBrackets > end)
                {
                    end = closeBrackets + 1;
                }
            }
        }
        token = strView.substr(start, end - start);

        result.emplace_back(token);
        start = end + 1;
    }
    if (start < strView.size())
        result.emplace_back(strView.substr(start)); // Add the last segment
    return result;
}

bool json::Util::isInteger(std::string_view str)
{
    if (str.empty())
        return false;
    size_t startPos = 0;
    if (str[0] == '+' || str[0] == '-')
        startPos = 1;

    if (startPos == str.size())
        return false; // string is only "+" or "-"

    return std::all_of(str.begin() + startPos, str.end(), ::isdigit);
}

bool json::Util::isDouble(std::string_view str)
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

bool json::Util::isBoolean(std::string_view str)
{
    return str == "true" || str == "false" || str == "True" || str == "False";
}

std::string_view json::Util::trim(std::string_view stringView)
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

int json::Util::toInteger(std::string_view value)
{
    int intValue;
    auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), intValue);
    if (ec != std::errc())
    {
        throw std::runtime_error("Conversion to int failed");
    }
    return intValue;
}

double json::Util::toDouble(std::string_view value)
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