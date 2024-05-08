//
// Created by Salomon Lee on 5/7/24.
//

#include "json_value.hpp"

std::string json::json_value::variant_to_string(
    const std::variant<int, bool, double, std::string, std::shared_ptr<json_object>, std::shared_ptr<json_array>> &var)
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
            } else if constexpr (std::is_same_v<T, std::shared_ptr<json_object>>) {
                ss << val->to_json_string();
            } else if constexpr (std::is_same_v<T, std::shared_ptr<json_array>>) {
                ss << val->to_json_string();
            } else {
                ss << "null";
            } }, var);
        return ss.str();
    }
}

std::ostream &json::operator<<(std::ostream &os, const json::json_value &v)
{
    os << json_value::variant_to_string(v.record);
    return os;
}

const std::variant<int, bool, double, std::string, std::shared_ptr<json::json_object>, std::shared_ptr<json::json_array>> &
json::json_value::getRecord() const
{
    return record;
}

void json::json_value::setRecord(
    const std::variant<int, bool, double, std::string, std::shared_ptr<json_object>, std::shared_ptr<json_array>> &record)
{
    json_value::record = record;
}

json::json_value::json_value(
    const std::variant<int, bool, double, std::string, std::shared_ptr<json_object>, std::shared_ptr<json_array>> &record)
    : record(record) {}
