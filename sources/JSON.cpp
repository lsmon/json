//
// Created by alcacruz on 5/24/24.
//
#include "JSON.hpp"
#include <algorithm>

JSON::JSON() : record(std::string()) {}

JSON::JSON(const std::variant<int, double, long, long long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &value)  {
    set((const std::variant<int, double, long, long long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &) value);
}

JSON::JSON(const std::shared_ptr<JSONObject> &jsonObject) : record(jsonObject) {}

JSON::JSON(const std::shared_ptr<JSONArray> &jsonArray) : record(jsonArray) {}

void JSON::set(
        const std::variant<int, double, long, long long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &r) {
    record = r;
}

std::variant<int, double, long, long long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>>
JSON::get() const {
    return record;
}

template<typename T>
bool JSON::is() const {
    return std::holds_alternative<T>(record);
}

template<class T>
bool JSON::isObject() const {
    return std::holds_alternative<std::shared_ptr<JSONObject>>(record) || std::holds_alternative<std::shared_ptr<JSONArray>>(record);
}

std::string JSON::str() const {
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

std::string JSON::c_str() const {
    std::string cstr = str();
    if (cstr.size() >= 2 && cstr.front() == '"' && cstr.back() == '"') {
        return cstr.substr(1, cstr.size() - 2); // Remove the first and last character
    }
    return cstr;
}

JSON JSON::operator=(const std::variant<int, double, long, long long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &r) {
    set(r);
    return *this;
}

std::ostream &operator<<(std::ostream &os, const JSON &value) {
    os << value.str();
    return os;
}

std::string JSON::dump(const int &indentSz, const int &currentSz) const {
    std::stringstream ss;
    if (std::holds_alternative<int>(record))
        ss << std::get<int>(record);
    else if (std::holds_alternative<long>(record))
        ss << std::get<long>(record);
    else if (std::holds_alternative<double>(record))
        ss << std::get<double>(record);
    else if (std::holds_alternative<bool>(record))
        ss << (std::get<bool>(record) ? "true" : "false");
    else if (std::holds_alternative<std::string>(record)) {
        std::string recordStr = std::get<std::string>(record);
        if (!recordStr.empty() && recordStr.front() != '"' && recordStr.back() != '"')
            ss << "\"" << recordStr << "\"";
        else
            ss << recordStr;
    } else if (std::holds_alternative<std::shared_ptr<JSONObject>>(record))
        ss << std::get<std::shared_ptr<JSONObject>>(record)->dump(indentSz, currentSz);
    else if (std::holds_alternative<std::shared_ptr<JSONArray>>(record))
        ss << std::get<std::shared_ptr<JSONArray>>(record)->dump(indentSz, currentSz);
    else
        ss << "null";
    return ss.str();
}

bool JSON::equals(const JSON &other) const
{
    if (std::holds_alternative<int>(record))
        return std::get<int>(record) == std::get<int>(other.record);
    else if (std::holds_alternative<long>(record))
        return std::get<long>(record) == std::get<long>(other.record);
    else if (std::holds_alternative<double>(record))
        return std::get<double>(record) == std::get<double>(other.record);
    else if (std::holds_alternative<bool>(record))
        return std::get<bool>(record) == std::get<bool>(other.record);
    else if (std::holds_alternative<std::string>(record))
        return std::get<std::string>(record) == std::get<std::string>(other.record);
    else if (std::holds_alternative<std::shared_ptr<JSONObject>>(record))
        return std::get<std::shared_ptr<JSONObject>>(record)->equals(*std::get<std::shared_ptr<JSONObject>>(other.record));
    else if (std::holds_alternative<std::shared_ptr<JSONArray>>(record))
        return std::get<std::shared_ptr<JSONArray>>(record)->equals(*std::get<std::shared_ptr<JSONArray>>(other.record));
    else
        return false;
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

void JSONArray::add(const JSON &value) {
    values.push_back(value);
}

void JSONArray::add(const std::variant<int, double, long, long long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &value) {
    values.emplace_back(value);
}

void JSONArray::add(const std::shared_ptr<JSONObject> &jsonObject) {
    values.emplace_back(jsonObject);
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

void JSONArray::sortBy(const std::string &key, const bool &asc) {
    if (asc) {
        std::sort(values.begin(), values.end(), [&key](const JSON &a, const JSON &b) {
            if (a.is<std::shared_ptr<JSONObject>>() && b.is<std::shared_ptr<JSONObject>>()) {
                auto objA = std::get<std::shared_ptr<JSONObject>>(a.get());
                auto objB = std::get<std::shared_ptr<JSONObject>>(b.get());

                if (objA->get().count(key) && objB->get().count(key)) {
                    auto valA = objA->operator[](key);
                    auto valB = objB->operator[](key);
                    return valA.c_str() < valB.c_str(); // Adjust comparison for data type
                }
            }
            return false; // Default to no swapping if key is not found
        });
    } else {
        std::sort(values.begin(), values.end(), [&key](const JSON &a, const JSON &b) {
            if (a.is<std::shared_ptr<JSONObject>>() && b.is<std::shared_ptr<JSONObject>>()) {
                auto objA = std::get<std::shared_ptr<JSONObject>>(a.get());
                auto objB = std::get<std::shared_ptr<JSONObject>>(b.get());

                if (objA->get().count(key) && objB->get().count(key)) {
                    auto valA = objA->operator[](key);
                    auto valB = objB->operator[](key);
                    return valA.c_str() > valB.c_str(); // Adjust comparison for data type
                }
            }
            return false; // Default to no swapping if key is not found
        });
    }
}

JSONArray JSONArray::operator=(const JSON &v) {
    add(v);
    return *this;
}

JSON &JSONArray::operator[](std::size_t index) {
    return values.at(index);
}

const JSON &JSONArray::operator[](std::size_t index) const {
    return values.at(index);
}

std::string JSONArray::dump(const int &indentSz, const int &currentSz) const {
    std::string indent(currentSz, ' ');                // Current indentation
    std::string innerIndent(currentSz + indentSz, ' '); // Next level indentation
    std::stringstream ss;
    ss << indent << "[\n";
    bool first = true;
    for (const auto &value : values) {
        if (!first) {
            ss << ",\n";
        }
        const int newIndentSz = currentSz + indentSz;
        if (value.isObject<JSONObject>() || value.isObject<JSONArray>()) {
            ss << value.dump(indentSz, newIndentSz);
        } else {
            ss << std::string(newIndentSz, ' ') << value.dump(0);
        }
        first = false;
    }
    ss << "\n" << indent << "]\n";
    return ss.str();
}

bool JSONArray::contains(const JSON &value) const
{
    for (const auto &val : values)
    {
        if (val.equals(value))
        {
            return true;
        }
    }
    return false;
}

bool JSONArray::contains(const std::variant<int, double, long, long long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &value) const
{
    return contains(JSON(value));
}

std::vector<JSON> JSONArray::get() const {
    return values;
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

void JSONObject::put(const std::string &key, const JSON &value) {
    object[key] = value;
}

void JSONObject::put(const std::string &key, const std::variant<int, double, long, long long, bool, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &r) {
    JSON v(r);
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

bool JSONObject::contains(const std::string &key) const
{
    return object.count(key) > 0;
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

JSON &JSONObject::operator[](const std::string &key) {
    return object[key];
}

const std::unordered_map<std::string, JSON> &JSONObject::get() const {
    return object;
}

std::string JSONObject::dump(const int &indentSz, const int &currentSz) const {
    std::string indent(currentSz, ' ');                // Current indentation
    std::string innerIndent(currentSz + indentSz, ' '); // Next level indentation

    std::stringstream ss;
    ss << "\n" << indent << "{\n";

    bool first = true;
    for (const auto &[key, value] : object) {
        if (!first) {
            ss << ",\n";
        }
        ss << innerIndent << "\"" << key << "\": ";
        if (value.isObject<JSONObject>() || value.isObject<JSONArray>()) {
            const int newIndentSz = currentSz + indentSz;
            ss << value.dump(indentSz, newIndentSz);
        } else {
            ss << value.dump(0);
        }
        first = false;
    }

    ss << "\n" << indent << "}";
    return ss.str();
}
