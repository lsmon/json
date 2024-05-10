//
// Created by Salomon Lee on 5/7/24.
//

#include "JSONValue.hpp"

std::string json::JSONValue::variantToString(
    const std::variant<int, bool, double, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &var)
{
    {
        std::stringstream ss;
        std::visit([&ss](const auto &val)
                   {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, int>) {
                ss << val;
            } else if constexpr (std::is_same_v<T, bool>) {
                ss << (val ? "true" : "false");
            } else if constexpr (std::is_same_v<T, double>) {
                ss << val;
            } else if constexpr (std::is_same_v<T, std::string>) {
                ss << val;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<JSONObject>>) {
                ss << val->toJSONString();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<JSONArray>>) {
                ss << val->toJSONString();
            } else {
                ss << "null";
            } }, var);
        return ss.str();
    }
}

std::ostream &json::operator<<(std::ostream &os, const json::JSONValue &v)
{
    os << JSONValue::variantToString(v.record);
    return os;
}

const std::variant<int, bool, double, std::string, std::shared_ptr<json::JSONObject>, std::shared_ptr<json::JSONArray>> &
json::JSONValue::getRecord() const
{
    return record;
}

void json::JSONValue::setRecord(
    const std::variant<int, bool, double, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &record)
{
    JSONValue::record = record;
}

json::JSONValue::JSONValue(
    const std::variant<int, bool, double, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &record)
    : record(record) {}


template <typename T>
bool json::JSONValue::is(const std::variant<int, bool, double, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>>& value) const
{
return std::get_if<T>(&value) != nullptr;
}

bool json::JSONValue::isObject(const std::string& value) const {
    return is<std::shared_ptr<JSONObject>>(value);
}

bool json::JSONValue::isArray(const std::string& value) const {
    return is<std::shared_ptr<JSONArray>>(value);
}

bool json::JSONValue::isString(const std::string& key) const {
    return is<std::string>(key);
}

bool json::JSONValue::isBool(const std::string& key) const {
    return is<bool>(key);
}

bool json::JSONValue::isNumber(const std::string& key) const {
    return is<int>(key) || is<double>(key);
}