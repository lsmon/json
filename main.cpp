#include "include/JSON.hpp"

int main() {
    // Example usage
    JSONArray innerArray1;
    innerArray1.add(Value(1));
    innerArray1.add(Value(2));
    JSONArray innerArray2;
    innerArray2.add(Value(3));
    innerArray2.add(Value(4));

    JSONArray jsonArray;
    jsonArray.add(Value(std::make_shared<JSONArray>(innerArray1)));
    jsonArray.add(Value(std::make_shared<JSONArray>(innerArray2)));

    std::cout << "JSONArray: " << jsonArray.str() << std::endl;

    JSONObject jsonObject;
    jsonObject.put("array1", Value(std::make_shared<JSONArray>(innerArray1)));
    jsonObject.put("array2", Value(std::make_shared<JSONArray>(innerArray2)));
    jsonObject.put("int", 10);
    jsonObject.put("double", 1.0134);
    jsonObject.put("bool", true);
    jsonObject.put("string", std::string("Hello world"));
    std::cout << "JSONObject: " << jsonObject.dump(2) << std::endl;

    auto objectNotation = Util::parse(jsonObject.str());
    std::cout << "Parsed JSON: " << objectNotation->dump(2) << std::endl;

    auto arrayNotation = Util::parse(jsonArray.str());
    std::cout << "Parsed Array: " << arrayNotation->dump(2) << std::endl;

    return 0;
}
