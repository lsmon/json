#ifndef JSON_JSONOBJECT_HPP
#define JSON_JSONOBJECT_HPP

#include <unordered_map>
#include <variant>
#include <memory>
#include "json_array.hpp"
#include "json_util.hpp"
#include "json_parser.hpp"

namespace json {
    class json_array;
     
    class json_object : public util {
    private:
        std::unordered_map<std::string, std::variant<int, bool, double, std::string, std::shared_ptr<json_object>, std::shared_ptr<json_array>>> key_value;

    public:
        json_object() = default;

        json_object(const json_object& obj);

        json_object(const std::string& jsonString);

        ~json_object();

        std::unordered_set<std::string> keys() const;

        void put(const std::string& key, const std::string& value);

        void put(const std::string& key, const char* value);

        void put(const std::string& key, bool value);

        void put(const std::string& key, int value);

        void put(const std::string& key, double value);

        void put(const std::string& key, const std::shared_ptr<json_object>& value);

        void put(const std::string& key, const std::shared_ptr<json_array>& value);

        template <typename T>
        T get(const std::string& key) const;

        std::string to_json_string() const override;

    };
}

#endif