//
// Created by alcacruz on 5/24/24.
//

#ifndef JSONPP_PARSER_HPP
#define JSONPP_PARSER_HPP


#include <mutex>
#include <future>
#include "Util.hpp"

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


#endif //JSONPP_PARSER_HPP
