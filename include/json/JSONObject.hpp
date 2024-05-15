#ifndef JSON_JSONOBJECT_HPP
#define JSON_JSONOBJECT_HPP

#include <unordered_map>
#include <memory>
#include "JSONArray.hpp"
#include "JSONValue.hpp"
#include "JSONUtil.hpp"
#include "JSONParser.hpp"

namespace json
{
    class JSONArray;
    class JSONValue;

    class JSONObject : public Util
    {
    private:
        std::unordered_map<std::string, JSONValue> key_value;

    public:
        JSONObject() = default;

        JSONObject(const JSONObject &obj);

        JSONObject(const std::string &jsonString);

        ~JSONObject();

        std::unordered_set<std::string> keys() const;

        void put(const std::string &key, const std::string &value);

        void put(const std::string &key, const char *value);

        void put(const std::string &key, bool value);

        void put(const std::string &key, int value);

        void put(const std::string &key, double value);

        void put(const std::string &key, const std::shared_ptr<JSONObject> &value);

        void put(const std::string &key, const std::shared_ptr<JSONArray> &value);

        template <typename T>
        T get(const std::string &key) const;

        std::string toJSONString() const override;

        JSONValue &operator[](const std::string &key);
    };
}

#endif