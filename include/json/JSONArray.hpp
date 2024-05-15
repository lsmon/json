#ifndef JSON_JSONARRAY_HPP
#define JSON_JSONARRAY_HPP

#include <vector>
#include <variant>
#include "JSONObject.hpp"
#include "JSONUtil.hpp"
#include "JSONValue.hpp"

namespace json
{
    class JSONObject;
    class JSONValue;
    class Util;

    class JSONArray : public Util
    {
    private:
        std::vector<json::JSONValue> elements;

    public:
        JSONArray() = default;

        JSONArray(const JSONArray &array);

        JSONArray(const std::string &jsonString);

        ~JSONArray() = default;

        size_t size() const;

        void add(const std::string &element);

        void add(const char *element);

        void add(const int &element);

        void add(const bool &element);

        void add(const double &element);

        void add(const std::shared_ptr<JSONObject> &element);

        void add(const std::shared_ptr<JSONArray> &element);

        template <typename T>
        T get(size_t index) const;

        std::string toJSONString() const override;

        JSONArray &operator<<(const std::string &element);

        JSONArray &operator<<(const bool &element);

        JSONArray &operator<<(const int &element);

        JSONArray &operator<<(const double &element);
    };
}

#endif