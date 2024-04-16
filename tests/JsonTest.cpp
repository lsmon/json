#include <iostream>
#include "json_parser.hpp"
#include "json_object.hpp"


void testSimpleJsonString();

void testNestedSimpleString();

void testSimpleJsonArray();

void testJsonArrayOfJsonObjects();

void testAsyncParser();

void testJsonObjectBasicOp();

void testJsonArrayBasicOp();

void testCreateJsonArrayFromString();

int main() {
    testSimpleJsonString();
    testNestedSimpleString();
    testSimpleJsonArray();
    testJsonArrayOfJsonObjects();
    testAsyncParser();
    testJsonObjectBasicOp();
    testJsonArrayBasicOp();
    testCreateJsonArrayFromString();
    return 0;
}

void testSimpleJsonString() {
    std::cout << "TESTING SIMPLE JSON STRING PARSING" << std::endl;
    std::string jsonString = R"( {"name": "John Doe", "city": "New York"} )";
    // json::json_object json = json::parser::parse_object(jsonString);
    // std::cout << json.to_json_string() << std::endl;
    auto jsonObj = json::util::parse(jsonString);
    std::cout << jsonObj->to_json_string() << std::endl;
}

void testNestedSimpleString() {
    std::cout << "TESTING NESTED JSON STRING PARSING" << std::endl;
    std::string jsonString = R"( {"name": "John Doe", "city_state": {"city":"New York", "state":"NY"}} )";
    auto jsonObj = json::util::parse(jsonString);
    std::cout << jsonObj->to_json_string() << std::endl;
}

void testSimpleJsonArray() {
    std::cout << "TESTING SIMPLE JSON ARRAY STRING PARSING" << std::endl;
    std::string jsonString = R"( ["name", 1, 3.14, false] )";
    auto json = json::util::parse(jsonString);
    std::cout << json->to_json_string() << std::endl;   
}

void testJsonArrayOfJsonObjects() {
    std::cout << "TESTING SIMPLE JSON ARRAY STRING PARSING" << std::endl;
    std::string jsonString = R"( [{"name":"John"}, {"age":1}, {"pi": 3.14}, {"truth":false}] )";
    auto json = json::util::parse(jsonString);
    std::cout << json->to_json_string() << std::endl;   
}

void testAsyncParser() {
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

void testJsonObjectBasicOp() {
    json::json_object* json = new json::json_object();
    json->put("a", "n");
    std::cout << json->to_json_string() << std::endl;
    delete json;
}

void testJsonArrayBasicOp() {
    json::json_array* json = new json::json_array();
    json->add("a");
    json->add("n");
    std::cout << json->to_json_string() << std::endl;
    delete json;
}

void testCreateJsonObjectFromString() {
    std::string jsonString = R"( {"name": "John Doe", "city_state": {"city":"New York", "state":"NY"}} )";
    auto json = json::json_object(jsonString);
    for (const auto key : json.keys()) {
        // auto value = json.get<std::string>(key);
        // std::cout << key << ": " << value << std::endl;
    }
}

void testCreateJsonArrayFromString() {
    std::string jsonString = R"( ["name", 1, 3.14, false] )";
    auto json = json::json_array(jsonString);
    for (size_t i = 0; i < json.size(); i++) {
        // auto value = json.get<std::string>(i);
        // std::cout << value << std::endl;
    }
}
