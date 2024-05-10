# JSON Parser Library

This is a lightweight C++ library for parsing JSON data. It provides classes `JSONObject` and `JSONArray` for handling JSON objects and arrays respectively. The library supports parsing JSON strings into these data structures, as well as converting them back to JSON strings.

## Features

- Parse JSON strings into `JSONObject` and `JSONArray` structures.
- Convert `JSONObject` and `JSONArray` structures back to JSON strings.
- Support for basic JSON data types: string, number (integer and floating-point), boolean, null.
- Ability to nest objects and arrays within each other.

## Usage

### 1. Parsing JSON

```cpp
#include "json_parser.hpp"

// Parse a JSON string into a JSONObject
std::string json_str = "{\"name\":\"John\",\"age\":30,\"city\":\"New York\"}";
json::JSONObject obj = json::parseObject(json_str);

// Parse a JSON string into a JSONArray
std::string json_array_str = "[1, 2, 3, 4, 5]";
json::JSONArray arr = json::parseArray(json_array_str);