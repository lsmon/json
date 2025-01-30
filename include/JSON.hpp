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

class JSON : public Util {
private:
    std::variant<int, double, long, long long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> record;

public:
    JSON();

    explicit JSON(const std::variant<int, double, long, long long, bool, std::string> & value);

    explicit JSON(const std::shared_ptr<JSONObject>& jsonObject);

    explicit JSON(const std::shared_ptr<JSONArray>& jsonArray);

    template<typename T> [[nodiscard]] bool is() const;

    template<class C> [[nodiscard]] bool isObject() const;

    void set(const std::variant<int, double, long, long long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>>& r);

    std::variant<int, double, long, long long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> get() const;

    std::string str() const;

    std::string c_str() const;

    std::string dump(const int &indentSz = 0, const int &currentSz=0) const;

    JSON operator=(const std::variant<int, double, long, long long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>>& r);

    friend std::ostream& operator<<(std::ostream& os, const JSON& value);
};

class JSONArray : public JSON {
private:
    std::vector<JSON> values;

public:
    JSONArray() = default;

    JSONArray(const std::string& jsonString);

    size_t size();

    void add(const JSON& value);

    void add(const std::variant<int, double, long, long long, bool, std::string> & r);

    void add(const std::shared_ptr<JSONObject> &jsonObject);

    std::string str() const override;

    std::string dump(const int &indentSz = 0, const int &currentSzz=0) const override;

    void sortBy(const std::string &key, const bool &asc = true);

    std::vector<JSON> get() const;

    JSONArray operator=(const JSON& v);

    JSON& operator[](std::size_t index);

    const JSON& operator[](std::size_t index) const;
};

class JSONObject : public JSON {
private:
    std::unordered_map<std::string, JSON> object;

public:
    JSONObject() = default;

    JSONObject(const std::string &jsonString);

    const std::unordered_map<std::string, JSON> &get() const;

    void put(const std::string& key, const JSON& value);

    void put(const std::string &key, const char *r);

    void put(const std::string& key, const std::variant<int, double, long, long long, bool, std::string> & r);

    std::vector<std::string> keys() const;

    std::string str() const override;

    std::string dump(const int &indentSz = 0, const int &currentSz = 0) const override; 

    JSON& operator[](const std::string& key);
};



#endif //JSONPP_JSON_HPP
