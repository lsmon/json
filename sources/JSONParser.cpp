#include "JSONParser.hpp"
#include <stack>
#include <iostream>

namespace json
{
    JSONArray Parser::parseArray(std::string_view jsonString)
    {
        if (!Util::validate(jsonString))
        {
            throw std::runtime_error("json string square braces not balanced");
        }
        if (jsonString.empty())
        {
            throw std::runtime_error("json string is empty");
        }
        size_t pos = 0;
        jsonString = Util::trim(jsonString);
        if (!(jsonString[pos] == OSB))
        {
            throw std::runtime_error("json array needs to start with square brackets '[]' or curly braces '{}' ");
        }
        JSONArray jsonArray;
        do
        {
            switch (jsonString[pos])
            {
            case '[':
            {
                jsonString = stripSquareBrackets(jsonString);
                std::vector<std::string_view> splits = splitStringViewBy(jsonString, ',');
                for (auto s : splits)
                {
                    if (s[0] == OCB)
                    {
                        jsonArray.add(std::make_shared<JSONObject>(parseObject(s)));
                    }
                    else if (s[0] == OSB)
                    {
                        jsonArray.add(std::make_shared<JSONArray>(parseArray(s)));
                    }
                    else
                    {
                        std::string_view element = trim(s);
                        if (isBoolean(element))
                        {
                            jsonArray.add((element == "true" || element == "True"));
                        }
                        else if (isInteger(element))
                        {
                            jsonArray.add(toInteger(element));
                        }
                        else if (isDouble(element))
                        {
                            jsonArray.add(toDouble(element));
                        }
                        else
                        {
                            jsonArray.add(std::string(element));
                        }
                    }
                }
                pos += jsonString.size();
            }
            break;
            }
            if (pos <= jsonString.size())
                pos++;
        } while (pos <= jsonString.size() && !((jsonString[pos] != CCB || jsonString[pos] != CSB)));

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
                    std::string_view key = trim(obj.substr(0, obj.find_first_of(':')));
                    std::string_view value = trim(obj.substr(obj.find_first_of(':') + 1));
                    if (value[0] == OCB)
                    {
                        jsonObj.put(std::string(key), std::make_shared<JSONObject>(parseObject(value)));
                    }
                    else if (value[0] == OSB)
                    {
                        jsonObj.put(std::string(key), std::make_shared<JSONArray>(parseArray(value)));
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
}