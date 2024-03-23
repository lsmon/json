#ifndef JSON_JSONPARSER_HPP
#define JSON_JSONPARSER_HPP

#include <unordered_map>
#include <unordered_set>
#include <string>
#include "json_util.hpp"
#include "json_object.hpp"
#include "json_array.hpp"

namespace json {
    class json_object;
    class json_array;

    class parser : public util{
    public:
        static json_array parse_array(std::string_view jsonString);

        static json_object parse_object(std::string_view jsonView);

        static bool validate(const std::string& jsonString);
    };
}

#endif