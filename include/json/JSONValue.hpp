//
// Created by Salomon Lee on 5/7/24.
//

#ifndef JSON_JSONVALUE_HPP
#define JSON_JSONVALUE_HPP

#include <memory>
#include <ostream>
#include "JSONObject.hpp"
#include "util/Variant.hpp"

namespace json {
    class JSONObject;
    class JSONArray;

    class JSONValue {
    private:
        Variant<int, bool, double, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> record;

    public:
        JSONValue() : record(std::string()) {} // Default to string, can be changed to another default type

        explicit JSONValue(
                const Variant<int, bool, double, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>> &record);

        static std::string variantToString(const Variant<int, bool, double, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>>& var) ;

        friend std::ostream& operator<<(std::ostream& os, const JSONValue& v);

        const Variant<int, bool, double, std::string, std::shared_ptr<json::JSONObject>, std::shared_ptr<json::JSONArray>> &
        getRecord() const;

        void setRecord(
                const Variant<int, bool, double, std::string, std::shared_ptr<json::JSONObject>, std::shared_ptr<json::JSONArray>> &record);

        template <typename T>
        bool is(const Variant<int, bool, double, std::string, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>>& value) const;
        // bool is() const;

        bool isObject(const std::string& value) const;

        bool isArray(const std::string& value) const;

        bool isString(const std::string& key) const;

        bool isBool(const std::string& key) const;

        bool isNumber(const std::string& key) const;
    };
}

#endif //JSON_JSONVALUE_HPP
