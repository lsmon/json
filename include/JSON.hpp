//
// Created by alcacruz on 5/23/24.
//

#ifndef JSONPP_JSON_HPP
#define JSONPP_JSON_HPP

#include <iostream>
#include <variant>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include "util/Util.hpp"

class JSONObject;
class JSONArray;

class Value : public Util {
private:
    std::variant<int, double, long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> record;

public:
    Value();

    explicit Value(const std::variant<int, double, long, bool, std::string> & value);

    explicit Value(const std::shared_ptr<JSONObject>& jsonObject);

    explicit Value(const std::shared_ptr<JSONArray>& jsonArray);

    template<typename T>
    bool is() const;

    void set(const std::variant<int, double, long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>>& r);

    std::variant<int, double, long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> get() const;

    std::string str() const;

    std::string dump(const int &indentSz) const;

    Value operator=(const std::variant<int, double, long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>>& r);

    friend std::ostream& operator<<(std::ostream& os, const Value& value);
};

class JSONArray : public Util {
private:
    std::vector<Value> values;

public:
    JSONArray() = default;

    JSONArray(const std::string& jsonString);

    size_t size();

    void add(const Value& value);

    void add(const std::string& value);

    std::string str() const override;

    std::string dump(const int &indentSz) const;

    JSONArray operator=(const Value& v);

    Value& operator[](std::size_t index);

    const Value& operator[](std::size_t index) const;
};

class JSONObject : public Util {
private:
    std::unordered_map<std::string, Value> object;

public:
    JSONObject() = default;

    JSONObject(const std::string &jsonString);

    const std::unordered_map<std::string, Value> &get() const;

    void put(const std::string& key, const Value& value);

    void put(const std::string &key, const char *r);

    void put(const std::string& key, const std::variant<int, double, long, bool, std::string> & r);

    std::vector<std::string> keys() const;

    std::string str() const override;

    std::string dump(const int &indentSz) const;

    Value& operator[](const std::string& key);
};



#endif //JSONPP_JSON_HPP
