#include <iostream>
#include "json_parser.hpp"
#include "json_object.hpp"


void testSimpleJsonString();

void testNestedSimpleString();

void testSimpleJsonArray();

void testJsonArrayOfJsonObjects();

int main() {
  testSimpleJsonString();
  testNestedSimpleString();
  testSimpleJsonArray();
  testJsonArrayOfJsonObjects();
    return 0;
}

/* 
int main() {

  return 0;
}
 */
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
