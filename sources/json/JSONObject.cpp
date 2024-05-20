#include <sstream>
#include <iostream>
#include "json/JSONObject.hpp"

namespace json
{
    JSONObject::JSONObject(const JSONObject &obj)
    {
        for (const auto &pair : obj.key_value)
        {
            pair.second.getRecord().visit([&](const auto &value)
                                          {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, std::shared_ptr<JSONObject>>)
            {
                key_value[pair.first].setRecord(std::make_shared<JSONObject>(*value));
            }
            else if constexpr (std::is_same_v<T, std::shared_ptr<JSONArray>>)
            {
                key_value[pair.first].setRecord(std::make_shared<JSONArray>(*value));
            }
            else
            {
                key_value[pair.first] = pair.second;
            } });
        }
    }

    JSONObject::JSONObject(const std::string &jsonString)
    {
        auto parsed = Parser::parse(jsonString);
        if (parsed)
        {
            auto obj = dynamic_cast<JSONObject *>(parsed.get());
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

    JSONObject::~JSONObject() {}

    std::unordered_set<std::string> JSONObject::keys() const
    {
        std::unordered_set<std::string> keys;
        keys.reserve(key_value.size());
        for (const auto pair : key_value)
        {
            keys.emplace(pair.first);
        }
        return keys;
    }

    void JSONObject::put(const std::string &key, const std::string &value)
    {
        key_value[key].setRecord(value);
    }

    void JSONObject::put(const std::string &key, const char *value)
    {
        key_value[key].setRecord(std::string(value));
    }

    void JSONObject::put(const std::string &key, bool value)
    {
        key_value[key].setRecord(value);
    }

    void JSONObject::put(const std::string &key, int value)
    {
        key_value[key].setRecord(value);
    }

    void JSONObject::put(const std::string &key, double value)
    {
        key_value[key].setRecord(value);
    }

    void JSONObject::put(const std::string &key, const std::shared_ptr<JSONObject> &value)
    {
        key_value[key].setRecord(value);
    }

    void JSONObject::put(const std::string &key, const std::shared_ptr<JSONArray> &value)
    {
        key_value[key].setRecord(value);
    }

    template <typename T>
    T JSONObject::get(const std::string &key) const
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

    std::string JSONObject::toJSONString() const
    {
        std::stringstream ss;
        ss << "{";
        size_t count = 0;
        for (const auto &pair : key_value)
        {
            ss << pair.first << ": ";
            pair.second.getRecord().visit([&ss](auto &&value)
                                          {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<T, std::string>)
                {
                    ss << value;
                }
                else if constexpr (std::is_same_v<T, int>)
                {
                    ss << value;
                }
                else if constexpr (std::is_same_v<T, bool>)
                {
                    ss << (value ? "true" : "false");
                }
                else if constexpr (std::is_same_v<T, double>)
                {
                    ss << value;
                }
                else if constexpr (std::is_same_v<T, std::shared_ptr<JSONObject>>)
                {
                    ss << value->toJSONString();
                }
                else if constexpr (std::is_same_v<T, std::shared_ptr<JSONArray>>)
                {
                    ss << value->toJSONString();
                }
                else
                {
                    ss << "null";
                } });

            if (count++ < key_value.size() - 1)
            {
                ss << ",";
            }
        }
        ss << "}";
        return ss.str();
    }

    JSONValue &JSONObject::operator[](const std::string &key)
    {
        static JSONValue empty_obj;
        auto it = key_value.find(key);
        if (it != key_value.end())
        {
            return it->second;
        }
        return empty_obj;
    }
}
