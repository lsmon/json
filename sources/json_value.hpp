//
// Created by Salomon Lee on 5/7/24.
//

#ifndef JSON_JSON_VALUE_HPP
#define JSON_JSON_VALUE_HPP


#include <variant>
#include <memory>
#include "json_object.hpp"

namespace json {
    class json_object;
    class json_array;

    class json_value {
    private:
        std::variant<int, bool, double, std::string, std::shared_ptr<json_object>, std::shared_ptr<json_array>> record;

    public:
        json_value() = default;

        explicit json_value(
                const std::variant<int, bool, double, std::string, std::shared_ptr<json_object>, std::shared_ptr<json_array>> &record);

        static std::string variant_to_string(const std::variant<int, bool, double, std::string, std::shared_ptr<json_object>, std::shared_ptr<json_array>>& var) ;

        friend std::ostream& operator<<(std::ostream& os, const json_value& v);

        const std::variant<int, bool, double, std::string, std::shared_ptr<json::json_object>, std::shared_ptr<json::json_array>> &
        getRecord() const;

        void setRecord(
                const std::variant<int, bool, double, std::string, std::shared_ptr<json::json_object>, std::shared_ptr<json::json_array>> &record);

        template <typename T>
        bool is(const std::variant<int, bool, double, std::string, std::shared_ptr<json_object>, std::shared_ptr<json_array>>& value) const {
            return std::get_if<T>(&value) != nullptr;
        }

        bool is_object(const std::string& value) const {
            return is<std::shared_ptr<json_object>>(value);
        }

        bool is_array(const std::string& value) const {
            return is<std::shared_ptr<json_array>>(value);
        }

        bool is_string(const std::string& key) const {
            return is<std::string>(key);
        }

        bool is_bool(const std::string& key) const {
            return is<bool>(key);
        }

        bool is_number(const std::string& key) const {
            return is<int>(key) || is<double>(key);
        }
    };
}

#endif //JSON_JSON_VALUE_HPP
