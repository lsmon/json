#include <sstream>
#include <iostream>
#include "json_object.hpp"

namespace json {
    json_object::json_object(const json_object &obj)
    {
        key_value = obj.key_value;
    }

    json_object::~json_object() {}

    void json_object::put(const std::string& key, const std::string& value) {
        key_value[key] = std::move(value);
    }

    void json_object::put(const std::string& key, bool value) {
        key_value[key] = value;
    }

    void json_object::put(const std::string& key, int value) {
        key_value[key] = value;
    }

    void json_object::put(const std::string& key, double value) {
        key_value[key] = value;
    }

    void json_object::put(const std::string& key, const json_object& value) {
        key_value[key] = value;
    }

    void json_object::put(const std::string& key, const json_array &value) {
        key_value[key] = value;
    }

    template<typename T>
    inline T json_object::get(const std::string & key) const {
        T value;
        auto it = key_value.find(key);
        if (it != key_value.end()) {
            try {
                // return std::get<T>(it.second);
                value = it->second;
            } catch (const std::bad_variant_access& ex) {
                std::cerr << ex.what() << std::endl;
                throw std::runtime_error("Type mismatch for key: " + key);
            }
        }
        return value;
    }

    std::string json_object::to_json_string() const {
        std::stringstream ss;
        ss << "{";
        size_t count = 0;
        for (const auto &pair : key_value) {
            ss << pair.first << ":";
            if (std::holds_alternative<int>(pair.second)) {
                ss << std::get<int>(pair.second);
            } else if (std::holds_alternative<double>(pair.second)) {
                ss << std::get<double>(pair.second);
            } else if (std::holds_alternative<bool>(pair.second)) {
                ss << (std::get<bool>(pair.second)?"true":"false");
            } else if (std::holds_alternative<std::string>(pair.second)) {
                ss << std::get<std::string>(pair.second);
            } else if (std::holds_alternative<json_object>(pair.second)) {
                ss << std::get<json_object>(pair.second).to_json_string();
            } else if (std::holds_alternative<json_array>(pair.second)) {
                ss << std::get<json_array>(pair.second).to_json_string();
            } else {
                ss << "null";
            }

            if (count++ < key_value.size()-1) {
                ss  << ",";
            }
        }
        ss <<"}";
        return ss.str();
    }
}
