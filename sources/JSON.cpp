//
// Created by alcacruz on 5/24/24.
//
#include "JSON.hpp"

Value::Value() : record(std::string()) {}

Value::Value(const std::variant<int, double, long, bool, std::string> &value)  {
    set((const std::variant<int, double, long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &) value);
}

Value::Value(const std::shared_ptr<JSONObject> &jsonObject) : record(jsonObject) {}

Value::Value(const std::shared_ptr<JSONArray> &jsonArray) : record(jsonArray) {}

void Value::set(
        const std::variant<int, double, long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &r) {
    record = r;
}

std::variant<int, double, long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>>
Value::get() const {
    return record;
}

template<typename T>
bool Value::is() const {
    return std::holds_alternative<T>(record);
}

std::string Value::str() const {
    std::stringstream ss;
    if (std::holds_alternative<int>(record))
        ss << std::get<int>(record);
    else if (std::holds_alternative<long>(record))
        ss << std::get<long>(record);
    else if (std::holds_alternative<double>(record))
        ss << std::get<double>(record);
    else if (std::holds_alternative<bool>(record))
        ss << (std::get<bool>(record) ? "true" : "false");
    else if (std::holds_alternative<std::string>(record))
        ss << "\"" << std::get<std::string>(record) << "\"";
    else if (std::holds_alternative<std::shared_ptr<JSONObject>>(record))
        ss << std::get<std::shared_ptr<JSONObject>>(record)->str();
    else if (std::holds_alternative<std::shared_ptr<JSONArray>>(record))
        ss << std::get<std::shared_ptr<JSONArray>>(record)->str();
    else
        ss << "null";
    return ss.str();
}

Value Value::operator=(
        const std::variant<int, double, long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &r) {
    set(r);
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Value &value) {
    os << value.str();
    return os;
}

std::string Value::dump(const int &indentSz) const {
    std::stringstream ss;
    if (std::holds_alternative<int>(record))
        ss << std::get<int>(record);
    else if (std::holds_alternative<long>(record))
        ss << std::get<long>(record);
    else if (std::holds_alternative<double>(record))
        ss << std::get<double>(record);
    else if (std::holds_alternative<bool>(record))
        ss << (std::get<bool>(record) ? "true" : "false");
    else if (std::holds_alternative<std::string>(record))
        ss << "\"" << std::get<std::string>(record) << "\"";
    else if (std::holds_alternative<std::shared_ptr<JSONObject>>(record))
        ss << std::get<std::shared_ptr<JSONObject>>(record)->dump(indentSz);
    else if (std::holds_alternative<std::shared_ptr<JSONArray>>(record))
        ss << std::get<std::shared_ptr<JSONArray>>(record)->dump(indentSz);
    else
        ss << "null";
    return ss.str();
}


JSONArray::JSONArray(const std::string &jsonString) {
    auto parsed = Util::parse(jsonString);
    if (parsed)
    {
        auto arr = dynamic_cast<JSONArray *>(parsed.get());
        *this = std::move(*arr);
    }
    else
    {
        throw std::runtime_error("JSON String parsing failed.");
    }
}

size_t JSONArray::size() {
    return values.size();
}

void JSONArray::add(const Value &value) {
    values.push_back(value);
}

void JSONArray::add(const std::string &value) {
    values.emplace_back(value);
}

std::string JSONArray::str() const {
    std::stringstream ss;
    ss << "[";
    bool first = true;
    for (const auto& value : values) {
        if (!first) {
            ss << ",";
        }
        ss << value.str();
        first = false;
    }
    ss << "]";
    return ss.str();
}

JSONArray JSONArray::operator=(const Value &v) {
    add(v);
    return *this;
}

Value &JSONArray::operator[](std::size_t index) {
    return values.at(index);
}

const Value &JSONArray::operator[](std::size_t index) const {
    return values.at(index);
}

std::string JSONArray::dump(const int &indentSz) const {
    std::string indent(indentSz, ' ');
    std::string innerIndent(indentSz + 2, ' '); // Increase indent for nested elements
    std::stringstream ss;
    ss << "[\n";
    bool first = true;
    for (const auto &value : values) {
        if (!first) {
            ss << ",\n";
        }
        ss << innerIndent << value.dump(indentSz + 2);
        first = false;
    }
    ss << "\n" << indent << "]";
    return ss.str();
}


JSONObject::JSONObject(const std::string &jsonString) {
    auto parsed = Util::parse(jsonString);
    if (parsed) {
        auto obj = dynamic_cast<JSONObject *>(parsed.get());
        *this = std::move(*obj);
    } else {
        throw std::runtime_error("JSON String parsing failed");
    }
}

void JSONObject::put(const std::string &key, const Value &value) {
    object[key] = value;
}

void JSONObject::put(const std::string &key, const std::variant<int, double, long, bool, std::string> &r) {
    Value v(r);
    object[key] = v;
}

void JSONObject::put(const std::string &key, const char *r) {
    put(key, std::string(r));
}

std::vector<std::string> JSONObject::keys() const
{
    std::vector<std::string> keys;
    keys.reserve(object.size());
    for (const auto pair : object)
    {
        keys.emplace_back(pair.first);
    }
    return keys;
}

std::string JSONObject::str() const {
    std::stringstream ss;
    ss << "{";
    bool first = true;
    for (const auto& [key, value] : object) {
        if (!first) {
            ss << ", ";
        }
        ss << "\"" << key << "\": " << value.str();
        first = false;
    }
    ss << "}";
    return ss.str();
}

Value &JSONObject::operator[](const std::string &key) {
    return object[key];
}

const std::unordered_map<std::string, Value> &JSONObject::get() const {
    return object;
}

std::string JSONObject::dump(const int &indentSz) const {
    std::string indent(indentSz, ' ');
    std::string innerIndent(indentSz + 2, ' '); // Increase indent for nested elements
    std::stringstream ss;
    ss << "{\n";
    bool first = true;
    for (const auto &[key, value] : object) {
        if (!first) {
            ss << ",\n";
        }
        ss << innerIndent << "\"" << key << "\": " << value.dump(indentSz + 2);
        first = false;
    }
    ss << "\n" << indent << "}";
    return ss.str();
}
