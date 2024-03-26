#include <sstream>
#include <iostream>
#include "json_object.hpp"

namespace json {
    json_object::json_object(const json_object &obj)
    {
        for (const auto& pair : obj.key_value) {
            if (auto inner_obj = std::get_if<std::shared_ptr<json_object>>(&pair.second)) {
                key_value[pair.first] = std::make_shared<json_object>(**inner_obj);
            } else if (auto inner_arr = std::get_if<std::shared_ptr<json_array>>(&pair.second)) {
                key_value[pair.first] = std::make_shared<json_array>(**inner_arr);
            } else {
                key_value[pair.first] = pair.second;
            }
        }
    }

    json_object::~json_object() {}

    void json_object::put(const std::string& key, const std::string& value) {
        key_value[key] = std::move(value);
    }

    void json_object::put(const std::string &key, const char *value)
    {
        key_value[key] = std::move(std::string(value));
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

    void json_object::put(const std::string& key, const std::shared_ptr<json_object>& value) {
        key_value[key] = value;
    }

    void json_object::put(const std::string& key, const std::shared_ptr<json_array> &value) {
        key_value[key] = value;
    }

    template<typename T>
    inline T json_object::get(const std::string & key) const {
        T value;
        auto it = key_value.find(key);
        if (it != key_value.end()) {
            try {
                value = std::get<T>(it->second);
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
            ss << "\"" << pair.first << "\": ";
            if (auto str = std::get_if<std::string>(&pair.second)) {
                ss << "\"" << *str << "\"";
            } else if (auto integer = std::get_if<int>(&pair.second)) {
                ss << *integer;
            } else if (auto boolean = std::get_if<bool>(&pair.second)) {
                ss << (*boolean ? "true" : "false");
            } else if (auto floating = std::get_if<double>(&pair.second)) {
                ss << *floating;
            } else if (auto obj = std::get_if<std::shared_ptr<json_object>>(&pair.second)) {
                ss << (*obj)->to_json_string();
            } else if (auto arr = std::get_if<std::shared_ptr<json_array>>(&pair.second)) {
                ss << (*arr)->to_json_string();
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
