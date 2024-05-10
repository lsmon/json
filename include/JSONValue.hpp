//
// Created by Salomon Lee on 5/7/24.
//

#ifndef JSON_JSONVALUE_HPP
#define JSON_JSONVALUE_HPP


#include <variant>
#include <memory>
#include "JSONObject.hpp"

namespace json {
    class JSONObject;
    class JSONArray;

    class JSONValue {
    private:
        std::variant<int, bool, double, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> record;

    public:
        JSONValue() = default;

        explicit JSONValue(
                const std::variant<int, bool, double, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &record);

        static std::string variantToString(const std::variant<int, bool, double, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>>& var) ;

        friend std::ostream& operator<<(std::ostream& os, const JSONValue& v);

        const std::variant<int, bool, double, std::string, std::shared_ptr<json::JSONObject>, std::shared_ptr<json::JSONArray>> &
        getRecord() const;

        void setRecord(
                const std::variant<int, bool, double, std::string, std::shared_ptr<json::JSONObject>, std::shared_ptr<json::JSONArray>> &record);

        template <typename T>
        bool is(const std::variant<int, bool, double, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>>& value) const;

        bool isObject(const std::string& value) const;

        bool isArray(const std::string& value) const;

        bool isString(const std::string& key) const;

        bool isBool(const std::string& key) const;

        bool isNumber(const std::string& key) const;
    };
}

#endif //JSON_JSONVALUE_HPP
