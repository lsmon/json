#include <sstream>
#include <iostream>
#include <variant>
#include "json_object.hpp"

namespace json
{
    json_object::json_object(const json_object &obj)
    {
        for (const auto &pair : obj.key_value)
        {
            if (auto inner_obj = std::get_if<std::shared_ptr<json_object>>(&pair.second.getRecord()))
            {
                key_value[pair.first].setRecord(std::make_shared<json_object>(**inner_obj));
            }
            else if (auto inner_arr = std::get_if<std::shared_ptr<json_array>>(&pair.second.getRecord()))
            {
                key_value[pair.first].setRecord(std::make_shared<json_array>(**inner_arr));
            }
            else
            {
                key_value[pair.first] = pair.second;
            }
        }
    }

    json_object::json_object(const std::string &jsonString)
    {
        auto parsed = parser::parse(jsonString);
        if (parsed)
        {
            auto obj = dynamic_cast<json_object *>(parsed.get());
            if (obj)
            {
                *this = std::move(*obj);
            }
            else
            {
                throw std::runtime_error("Parsed string is not a json_object");
            }
        }
        else
        {
            throw std::runtime_error("JSON String parsing failed.");
        }
    }

    json_object::~json_object() {}

    std::unordered_set<std::string> json_object::keys() const
    {
        std::unordered_set<std::string> keys;
        keys.reserve(key_value.size());
        for (const auto pair : key_value)
        {
            keys.emplace(pair.first);
        }
        return keys;
    }

    void json_object::put(const std::string &key, const std::string &value)
    {
        key_value[key].setRecord(value);
    }

    void json_object::put(const std::string &key, const char *value)
    {
        key_value[key].setRecord(std::string(value));
    }

    void json_object::put(const std::string &key, bool value)
    {
        key_value[key].setRecord(value);
    }

    void json_object::put(const std::string &key, int value)
    {
        key_value[key].setRecord(value);
    }

    void json_object::put(const std::string &key, double value)
    {
        key_value[key].setRecord(value);
    }

    void json_object::put(const std::string &key, const std::shared_ptr<json_object> &value)
    {
        key_value[key].setRecord(value);
    }

    void json_object::put(const std::string &key, const std::shared_ptr<json_array> &value)
    {
        key_value[key].setRecord(value);
    }

    template <typename T>
    T json_object::get(const std::string &key) const
    {
        auto it = key_value.find(key);
        if (it != key_value.end())
        {
            try
            {
                return std::get<T>(it->second);
            }
            catch (const std::bad_variant_access &ex)
            {
                std::cerr << ex.what() << std::endl;
                throw std::runtime_error("Type mismatch for key: " + key);
            }
        }
        else
        {
            throw std::out_of_range("Key not found");
        }
    }

    std::string json_object::to_json_string() const
    {
        std::stringstream ss;
        ss << "{";
        size_t count = 0;
        for (const auto &pair : key_value)
        {
            ss << pair.first << ": ";
            if (auto str = std::get_if<std::string>(&pair.second.getRecord()))
            {
                ss << *str;
            }
            else if (auto integer = std::get_if<int>(&pair.second.getRecord()))
            {
                ss << *integer;
            }
            else if (auto boolean = std::get_if<bool>(&pair.second.getRecord()))
            {
                ss << (*boolean ? "true" : "false");
            }
            else if (auto floating = std::get_if<double>(&pair.second.getRecord()))
            {
                ss << *floating;
            }
            else if (auto obj = std::get_if<std::shared_ptr<json_object>>(&pair.second.getRecord()))
            {
                ss << (*obj)->to_json_string();
            }
            else if (auto arr = std::get_if<std::shared_ptr<json_array>>(&pair.second.getRecord()))
            {
                ss << (*arr)->to_json_string();
            }
            else
            {
                ss << "null";
            }

            if (count++ < key_value.size() - 1)
            {
                ss << ",";
            }
        }
        ss << "}";
        return ss.str();
    }

    json_value &json_object::operator[](const std::string &key)
    {
        static json_value empty_obj;
        auto it = key_value.find(key);
        if (it != key_value.end())
        {
            return it->second;
        }
        return empty_obj;
    }

    bool json_object::is_array(const std::basic_string<char> basicString)
    {
        return false;
    }
}
