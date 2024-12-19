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
             auto value = json[i];
             std::cout << value << std::endl;
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
    JSONObject json;
    json.put("key", "value");
    std::cout << json.dump(2) << std::endl;
    JSONObject jsonObject( R"({"took":2,"timed_out":false,"_shards":{"total":2,"successful":2,"failed":0},"hits":{"total":621,"max_score":1.0,"hits":[{"_index":"screen","_type":"data","_id":"AZJD6gqXHwURsIsgy6Xt","_score":1.0,"_source":{"st":"vod","c":"screen_change","sw":0,"fn":45,"clid":"","cm":"","ua":"iPhone","fs":1,"ra":"74.108.50.211","dt":"2024-09-27T20:58:43.059Z","dvc":"iPhone17.7","uid":"001119.5b1661ec78e549dabe84853836524fb8.0715","api_key":"API_KEY ZW1haWw9c2Fsb21vbi5sZWVAYWxjYWNydXouY29tOmNvbXBhbnk9QWxjYWNydXo6Zmlyc3RfbmFtZT1TYWxvbW9uOmxhc3RfbmFtZT1MZWU6cmVnaXN0cmF0aW9uX2R0PTE0NzU3NzUxMzI5NDA=","vm":2,"wt":4677,"cid":"1f3c01c8-003f-11ef-b074-1b6763da968e","ts":1727470723062}},{"_index":"screen","_type":"data","_id":"AZGAp258HwURsIsgy59M","_score":1.0,"_source":{"st":"vod","c":"rtqa","sw":0,"fn":247,"clid":"5B6F52E2-5CBF-4053-829B-405E4E175023","cm":"","ua":"iOS","ra":"98.45.246.61","dt":"2023-05-23T22:39:59.813Z","dvc":"iPad7,3","api_key":"7d3568e5f1126764ac2c249f5240315441d725793d1b7b638f63e3d20769f381012a2cf89ae716f419404be8dd11153be9c5bada35490747faf38b262bd55270","vm":2,"wt":11000,"cid":"f64bbe50-ee12-11ed-afad-cb3e8b054e2b","ts":1684881599813}},{"_index":"screen","_type":"data","_id":"AZHZ6zZrHwURsIsgy6D-","_score":1.0,"_source":{"st":"vod","c":"rtqa","sw":0,"fn":247,"clid":"5B6F52E2-5CBF-4053-829B-405E4E175023","cm":"","ua":"iOS","ra":"98.45.246.61","dt":"2023-05-23T22:39:59.813Z","dvc":"iPad7,3","api_key":"7d3568e5f1126764ac2c249f5240315441d725793d1b7b638f63e3d20769f381012a2cf89ae716f419404be8dd11153be9c5bada35490747faf38b262bd55270","vm":2,"wt":11000,"cid":"f64bbe50-ee12-11ed-afad-cb3e8b054e2b","ts":1684881599813}},{"_index":"screen","_type":"data","_id":"AZHno69kHwURsIsgy6EX","_score":1.0,"_source":{"st":"vod","c":"rtqa","sw":0,"fn":247,"clid":"5B6F52E2-5CBF-4053-829B-405E4E175023","cm":"","ua":"iOS","ra":"98.45.246.61","dt":"2023-05-23T22:39:59.813Z","dvc":"iPad7,3","uid":"112098189963342649311","api_key":"7d3568e5f1126764ac2c249f5240315441d725793d1b7b638f63e3d20769f381012a2cf89ae716f419404be8dd11153be9c5bada35490747faf38b262bd55270","vm":2,"wt":11000,"cid":"f64bbe50-ee12-11ed-afad-cb3e8b054e2b","ts":1684881599813}},{"_index":"screen","_type":"data","_id":"AZHeYfiyHwURsIsgy6EG","_score":1.0,"_source":{"st":"vod","c":"rtqa","sw":0,"fn":247,"clid":"5B6F52E2-5CBF-4053-829B-405E4E175023","cm":"","ua":"iOS","ra":"98.45.246.61","dt":"2023-05-23T22:39:59.813Z","dvc":"iPad7,3","api_key":"7d3568e5f1126764ac2c249f5240315441d725793d1b7b638f63e3d20769f381012a2cf89ae716f419404be8dd11153be9c5bada35490747faf38b262bd55270","vm":2,"wt":11000,"cid":"f64bbe50-ee12-11ed-afad-cb3e8b054e2b","ts":1684881599813}},{"_index":"screen","_type":"data","_id":"AZHewZA0HwURsIsgy6EJ","_score":1.0,"_source":{"st":"vod","c":"rtqa","sw":0,"fn":247,"clid":"5B6F52E2-5CBF-4053-829B-405E4E175023","cm":"","ua":"iOS","ra":"98.45.246.61","dt":"2023-05-23T22:39:59.813Z","dvc":"iPad7,3","api_key":"7d3568e5f1126764ac2c249f5240315441d725793d1b7b638f63e3d20769f381012a2cf89ae716f419404be8dd11153be9c5bada35490747faf38b262bd55270","vm":2,"wt":11000,"cid":"f64bbe50-ee12-11ed-afad-cb3e8b054e2b","ts":1684881599813}},{"_index":"screen","_type":"data","_id":"AZF8yxUgHwURsIsgy59D","_score":1.0,"_source":{"st":"vod","c":"rtqa","sw":0,"fn":247,"clid":"5B6F52E2-5CBF-4053-829B-405E4E175023","cm":"","ua":"iOS","ra":"98.45.246.61","dt":"2023-05-23T22:39:59.813Z","dvc":"iPad7,3","api_key":"7d3568e5f1126764ac2c249f5240315441d725793d1b7b638f63e3d20769f381012a2cf89ae716f419404be8dd11153be9c5bada35490747faf38b262bd55270","vm":2,"wt":11000,"cid":"f64bbe50-ee12-11ed-afad-cb3e8b054e2b","ts":1684881599813}},{"_index":"screen","_type":"data","_id":"AZHiQyMU6G5Dxy1268kI","_score":1.0,"_source":{"st":"vod","c":"rtqa","sw":0,"fn":247,"clid":"5B6F52E2-5CBF-4053-829B-405E4E175023","cm":"","ua":"iOS","ra":"98.45.246.61","dt":"2023-05-23T22:39:59.813Z","dvc":"iPad7,3","api_key":"7d3568e5f1126764ac2c249f5240315441d725793d1b7b638f63e3d20769f381012a2cf89ae716f419404be8dd11153be9c5bada35490747faf38b262bd55270","vm":2,"wt":11000,"cid":"f64bbe50-ee12-11ed-afad-cb3e8b054e2b","ts":1684881599813}},{"_index":"screen","_type":"data","_id":"AZHnvDoMHwURsIsgy6EZ","_score":1.0,"_source":{"st":"vod","c":"rtqa","sw":0,"fn":247,"clid":"5B6F52E2-5CBF-4053-829B-405E4E175023","cm":"","ua":"iOS","ra":"98.45.246.61","dt":"2023-05-23T22:39:59.813Z","dvc":"iPad7,3","uid":"112098189963342649311","api_key":"7d3568e5f1126764ac2c249f5240315441d725793d1b7b638f63e3d20769f381012a2cf89ae716f419404be8dd11153be9c5bada35490747faf38b262bd55270","vm":2,"wt":11000,"cid":"f64bbe50-ee12-11ed-afad-cb3e8b054e2b","ts":1684881599813}},{"_index":"screen","_type":"data","_id":"AZHJcgOgHwURsIsgy6Dl","_score":1.0,"_source":{"st":"vod","c":"rtqa","sw":0,"fn":247,"clid":"5B6F52E2-5CBF-4053-829B-405E4E175023","cm":"","ua":"iOS","ra":"98.45.246.61","dt":"2023-05-23T22:39:59.813Z","dvc":"iPad7,3","api_key":"7d3568e5f1126764ac2c249f5240315441d725793d1b7b638f63e3d20769f381012a2cf89ae716f419404be8dd11153be9c5bada35490747faf38b262bd55270","vm":2,"wt":11000,"cid":"f64bbe50-ee12-11ed-afad-cb3e8b054e2b","ts":1684881599813}}]}})");
    std::cout << jsonObject.dump(4) << std::endl;
    return 0;
}
