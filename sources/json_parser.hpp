#ifndef JSON_JSONPARSER_HPP
#define JSON_JSONPARSER_HPP

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <future>
#include "json_util.hpp"
#include "json_object.hpp"
#include "json_array.hpp"

namespace json {
    extern std::mutex parser_mutex;

    class json_object;
    class json_array;

    class parser : public util{
    private:
        

    public:
        static json_array parse_array(std::string_view jsonString);

        static json_object parse_object(std::string_view jsonView);

        static bool validate(const std::string& jsonString);

        // Helper function for asynchronous parsing
        template<typename Function, typename... Args>
        static auto async_parse(Function&& f, Args&&... args)
        {
            return std::async(std::launch::async, std::forward<Function>(f), std::forward<Args>(args)...);
        }

        static std::future<json_array> async_parse_array(std::string_view jsonView);

        static std::future<json_object> async_parse_object(std::string_view jsonView);
    };

    
}

#endif