#include <iostream>
#include "json/JSONParser.hpp"


json::JSONObject obj;
json::JSONArray arr;

void createJsonObject() {
    obj.put("name", "John");
    obj.put("age", 30);
    obj.put("city", "New York");
}

void createJsonArray() {
    arr.add("apple");
    arr.add("banana");
    arr.add("orange");
}



int main(int argc, char* argv[]) {
    createJsonObject();
    createJsonArray();

    std::cout << obj.toJSONString() << std::endl;
    std::cout << arr.toJSONString() << std::endl;

    // std::string strVal = obj.get<std::string>("name");
    // std::cout << strVal << std::endl;
    // // std::cout << arr.get<std::string>(1) << std::endl;
    return 0;
}
