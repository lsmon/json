//
// Created by alcacruz on 5/24/24.
//

#include "util/Parser.hpp"
#include "JSON.hpp"
#include <memory>

JSONArray Parser::parseArray(std::string_view jsonString)
{
    try {
        if (!Util::validate(jsonString)) {
            throw std::runtime_error("json string square braces not balanced");
        }
    } catch (std::runtime_error &ex) {
        throw std::runtime_error(ex.what());
    }
    if (jsonString.empty()) {
        throw std::runtime_error("json string is empty");
    }
    size_t pos = 0;
    jsonString = Util::trimQuotes(Util::trim(jsonString));
    if (jsonString[pos] != '[') {
        throw std::runtime_error("json array needs to start with square brackets '[]' or curly braces '{}' ");
    }
    JSONArray jsonArray;
    std::string_view stringView = stripSquareBrackets(jsonString);
    jsonString = stringView;
    std::vector<std::string_view> splits = Util::splitStringViewBy(jsonString, ',');
    for (auto s : splits) {
        s = Util::trim(s);
        if (s.empty()) continue;
        if (s[0] == '{') {
            jsonArray.add(JSON(std::make_shared<JSONObject>(parseObject(s))));
        } else if (s[0] == '[') {
            jsonArray.add(JSON(std::make_shared<JSONArray>(parseArray(s))));
        } else {
            if (Util::isBoolean(s)) {
                jsonArray.add(JSON(s == "true" || s == "True"));
            } else if (Util::isInteger(s)) {
                jsonArray.add(JSON(Util::toInteger(s)));
            } else if (Util::isLong(s)) {
                jsonArray.add(JSON(Util::toLong(s)));
            } else if (Util::isDouble(s)) {
                jsonArray.add(JSON(Util::toDouble(s)));
            } else {
                jsonArray.add(JSON(std::string(s)));
            }
        }
    }
    return jsonArray;
}

JSONObject Parser::parseObject(std::string_view jsonView)
{
    JSONObject jsonObj;
    if (!Util::validate(jsonView))
    {
        throw std::runtime_error("json string brackets not balanced");
    }
    if (jsonView.empty())
    {
        throw std::runtime_error("json string is empty");
    }
    size_t pos = 0;
    jsonView = Util::trim(jsonView);
    if (!(jsonView[pos] == OCB || jsonView[pos] == OSB))
    {
        throw std::runtime_error("json string needs to start with square brackets '[' or curly braces '{' ");
    }
    do
    {
        switch (jsonView[pos])
        {
            case '{':
            {
                jsonView = stripCurlyBraces(jsonView);
                std::vector<std::string_view> splits = splitStringViewBy(jsonView, ',');

                for (std::string_view obj : splits)
                {
                    obj = trim(obj);
                    std::string_view key = trimQuotes(trim(obj.substr(0, obj.find_first_of(':'))));
                    std::string_view value = trimQuotes(trim(obj.substr(obj.find_first_of(':') + 1)));
                    if (value[0] == OCB)
                    {
                        jsonObj.put(std::string(key), JSON(std::make_shared<JSONObject>(parseObject(value))));
                    }
                    else if (value[0] == OSB)
                    {
                        JSON v(std::make_shared<JSONArray>(parseArray(value)));
                        jsonObj.put(std::string(key), v);
                    }
                    else
                    {
                        value = Util::trim(value);
                        if (isBoolean(value))
                        {
                            jsonObj.put(std::string(key), (value == "true" || value == "True"));
                        }
                        else if (isInteger(value))
                        {
                            jsonObj.put(std::string(key), toInteger(value));
                        }
                        else if (isLong(value))
                        {
                            jsonObj.put(std::string(key), toLong(value));
                        }
                        else if (isLongLong(value))
                        {
                            jsonObj.put(std::string(key), toLongLong(value));
                        }
                        else if (isDouble(value))
                        {
                            jsonObj.put(std::string(key), toDouble(value));
                        }
                        else
                        {
                            jsonObj.put(std::string(key), std::string(value));
                        }
                    }
                    pos += obj.size();
                }
            }
                break;
        }
        if (pos <= jsonView.size())
            pos++;
    } while (pos <= jsonView.size() && !((jsonView[pos] != CCB || jsonView[pos] != CSB)));
    return jsonObj;
}

bool Parser::validate(const std::string &jsonString)
{
    std::string_view stringView(jsonString);
    return Util::validate(stringView);
}

std::future<JSONArray> Parser::asyncParseArray(std::string_view jsonView)
{
    return asyncParse(parseArray, jsonView); // Utilize the async_parse helper function
}

std::future<JSONObject> Parser::asyncParseObject(std::string_view jsonView)
{
    return asyncParse(parseObject, jsonView); // Utilize the async_parse helper function
}