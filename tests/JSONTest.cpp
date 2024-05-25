//
// Created by alcacruz on 5/24/24.
//
#include <iostream>
#include "JSON.hpp"
#include "util/Parser.hpp"
#include "util/Util.hpp"
#include <unordered_map>
#include <string_view>

void testSimpleJsonString()
{
    std::cout << "TESTING SIMPLE JSON STRING PARSING" << std::endl;
    std::string jsonString = R"( {"name": "John Doe", "city": "New York"} )";
    // json_object json = parser::parse_object(jsonString);
    // std::cout << json.to_json_string() << std::endl;
    auto jsonObj = Util::parse(jsonString);
    std::cout << jsonObj->str() << std::endl;
}

void testNestedSimpleString()
{
    std::cout << "TESTING NESTED JSON STRING PARSING" << std::endl;
    std::string jsonString = R"( {"name": "John Doe", "city_state": {"city":"New York", "state":"NY"}} )";
    auto jsonObj = Util::parse(jsonString);
    std::cout << jsonObj->str() << std::endl;
}

void testSimpleJsonArray()
{
    std::cout << "TESTING SIMPLE JSON ARRAY STRING PARSING" << std::endl;
    std::string jsonString = R"( ["name", 1, 3.14, false] )";
    auto json = Util::parse(jsonString);
    std::cout << json->str() << std::endl;
}

void testJsonArrayOfJsonObjects()
{
    std::cout << "TESTING SIMPLE JSON ARRAY STRING PARSING" << std::endl;
    std::string jsonString = R"( [{"name":"John"}, {"age":1}, {"pi": 3.14}, {"truth":false}] )";
    auto json = Util::parse(jsonString);
    std::cout << json->str() << std::endl;
}

void testAsyncParser()
{
    std::cout << "TESTING ASYNC JSON PARSER" << std::endl;

    // Example usage
    std::string jsonStr1 = "[1, 2, 3]";
    std::string jsonStr2 = "{\"key\": \"value\"}";

    auto future1 = Parser::asyncParse(&Parser::parseArray, jsonStr1);
    auto future2 = Parser::asyncParse(&Parser::parseObject, jsonStr2);

    // Wait for the results
    auto jsonArray = future1.get();
    auto jsonObject = future2.get();

    std::cout << jsonArray.str() << std::endl;
    std::cout << jsonObject.str() << std::endl;
}

void testJsonObjectBasicOp()
{
    std::cout << "TESTING JSON OBJECT BASIC OPERATIONS" << std::endl;
    JSONObject *json = new JSONObject();
    json->put("a", "n");
    std::cout << json->str() << std::endl;
    delete json;
}

void testJsonArrayBasicOp()
{
    std::cout << "TESTING JSON ARRAY BASIC OPERATIONS" << std::endl;
    JSONArray *json = new JSONArray();
    json->add("a");
    json->add("n");
    std::cout << json->str() << std::endl;
    delete json;
}

void testCreateJsonObjectFromString()
{
    std::cout << "TESTING JSON OBJECT FROM STRING" << std::endl;
    std::string jsonString = R"( {"name": "John Doe", "city_state": {"city":"New York", "state":"NY"}} )";
    JSONObject json = JSONObject(jsonString);
    std::cout << json.str() << std::endl;
    for (const auto key : json.keys())
    {
        auto value = json[key];
        std::cout << key << ": " << value << std::endl;
    }
}

void testCreateJsonArrayFromString()
{
    std::cout << "TESTING JSON ARRAY FROM STRING" << std::endl;
    std::string jsonString = R"( ["name", 1, 3.14, false] )";
    JSONArray json = JSONArray(jsonString);
    std::cout << json.str() << std::endl;
    for (size_t i = 0; i < json.size(); i++)
    {
        //     auto value = json.get<std::string>(i);
        //     std::cout << value << std::endl;
    }
}

void wholeBundle()
{
    testSimpleJsonString();
    testNestedSimpleString();
    testSimpleJsonArray();
    testJsonArrayOfJsonObjects();
    testAsyncParser();
    testJsonObjectBasicOp();
    testJsonArrayBasicOp();
    testCreateJsonObjectFromString();
    testCreateJsonArrayFromString();
}

void handleArguments(const std::string_view &arg)
{
    static const std::unordered_map<std::size_t, void (*)()> argMap = {
            {std::hash<std::string_view>{}("simple"), []()
                                                      { testSimpleJsonString(); }},
            {std::hash<std::string_view>{}("nested"), []()
                                                      { testNestedSimpleString(); }},
            {std::hash<std::string_view>{}("array"), []()
                                                      { testSimpleJsonArray(); }},
            {std::hash<std::string_view>{}("array-of-objects"), []()
                                                      { testJsonArrayOfJsonObjects(); }},
            {std::hash<std::string_view>{}("async-parse"), []()
                                                      { testAsyncParser(); }},
            {std::hash<std::string_view>{}("object-basic-op"), []()
                                                      { testJsonObjectBasicOp(); }},
            {std::hash<std::string_view>{}("array-basic-op"), []()
                                                      { testJsonArrayBasicOp(); }},
            {std::hash<std::string_view>{}("string-to-object"), []()
                                                      { testCreateJsonObjectFromString(); }},
            {std::hash<std::string_view>{}("string-to-array"), []()
                                                      { testCreateJsonArrayFromString(); }}};

    auto it = argMap.find(std::hash<std::string_view>{}(arg));
    if (it != argMap.end())
    {
        it->second(); // Call the corresponding function
    }
    else
    {
        wholeBundle();
    }
}

int main(int argc, char **argv)
{
    std::vector<std::string_view> args(argv + 1, argv + argc);

    if (argc == 1)
    {
        wholeBundle();
    }
    for (std::string_view v : args)
    {
        handleArguments(v);
    }

    return 0;
}
