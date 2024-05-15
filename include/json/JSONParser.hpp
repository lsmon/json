#ifndef JSON_JSONPARSER_HPP
#define JSON_JSONPARSER_HPP

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <future>
#include "JSONUtil.hpp"
#include "JSONObject.hpp"
#include "JSONArray.hpp"

namespace json
{
    extern std::mutex parser_mutex;

    class JSONObject;
    class JSONArray;

    class Parser : public Util
    {
    private:
    public:
        static JSONArray parseArray(std::string_view jsonString);

        static JSONObject parseObject(std::string_view jsonView);

        static bool validate(const std::string &jsonString);

        // Helper function for asynchronous parsing
        template <typename Function, typename... Args>
        static auto asyncParse(Function &&f, Args &&...args)
        {
            return std::async(std::launch::async, std::forward<Function>(f), std::forward<Args>(args)...);
        }

        static std::future<JSONArray> asyncParseArray(std::string_view jsonView);

        static std::future<JSONObject> asyncParseObject(std::string_view jsonView);
    };

}

#endif