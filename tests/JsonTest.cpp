#include <iostream>
#include "json_parser.hpp"
#include "json_object.hpp"
#include <unordered_map>
#include <string_view>

void testSimpleJsonString()
{
    std::cout << "TESTING SIMPLE JSON STRING PARSING" << std::endl;
    std::string jsonString = R"( {"name": "John Doe", "city": "New York"} )";
    // json::json_object json = json::parser::parse_object(jsonString);
    // std::cout << json.to_json_string() << std::endl;
    auto jsonObj = json::util::parse(jsonString);
    std::cout << jsonObj->to_json_string() << std::endl;
}

void testNestedSimpleString()
{
    std::cout << "TESTING NESTED JSON STRING PARSING" << std::endl;
    std::string jsonString = R"( {"name": "John Doe", "city_state": {"city":"New York", "state":"NY"}} )";
    auto jsonObj = json::util::parse(jsonString);
    std::cout << jsonObj->to_json_string() << std::endl;
}

void testSimpleJsonArray()
{
    std::cout << "TESTING SIMPLE JSON ARRAY STRING PARSING" << std::endl;
    std::string jsonString = R"( ["name", 1, 3.14, false] )";
    auto json = json::util::parse(jsonString);
    std::cout << json->to_json_string() << std::endl;
}

void testJsonArrayOfJsonObjects()
{
    std::cout << "TESTING SIMPLE JSON ARRAY STRING PARSING" << std::endl;
    std::string jsonString = R"( [{"name":"John"}, {"age":1}, {"pi": 3.14}, {"truth":false}] )";
    auto json = json::util::parse(jsonString);
    std::cout << json->to_json_string() << std::endl;
}

void testAsyncParser()
{
    std::cout << "TESTING ASYNC JSON PARSER" << std::endl;

    // Example usage
    std::string jsonStr1 = "[1, 2, 3]";
    std::string jsonStr2 = "{\"key\": \"value\"}";

    auto future1 = json::parser::async_parse(&json::parser::parse_array, jsonStr1);
    auto future2 = json::parser::async_parse(&json::parser::parse_object, jsonStr2);

    // Wait for the results
    auto jsonArray = future1.get();
    auto jsonObject = future2.get();

    std::cout << jsonArray.to_json_string() << std::endl;
    std::cout << jsonObject.to_json_string() << std::endl;
}

void testJsonObjectBasicOp()
{
    std::cout << "TESTING JSON OBJECT BASIC OPERATIONS" << std::endl;
    json::json_object *json = new json::json_object();
    json->put("a", "n");
    std::cout << json->to_json_string() << std::endl;
    delete json;
}

void testJsonArrayBasicOp()
{
    std::cout << "TESTING JSON ARRAY BASIC OPERATIONS" << std::endl;
    json::json_array *json = new json::json_array();
    json->add("a");
    json->add("n");
    std::cout << json->to_json_string() << std::endl;
    delete json;
}

void testCreateJsonObjectFromString()
{
    std::cout << "TESTING JSON OBJECT FROM STRING" << std::endl;
    std::string jsonString = R"( {"name": "John Doe", "city_state": {"city":"New York", "state":"NY"}} )";
    json::json_object json = json::json_object(jsonString);
    std::cout << json.to_json_string() << std::endl;
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
    json::json_array json = json::json_array(jsonString);
    std::cout << json.to_json_string() << std::endl;
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
