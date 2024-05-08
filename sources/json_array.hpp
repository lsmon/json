#ifndef JSON_JSONARRAY_HPP
#define JSON_JSONARRAY_HPP

#include <vector>
#include <variant>
#include "json_object.hpp"
#include "json_util.hpp"
#include "json_value.hpp"

namespace json
{
    class json_object;
    class json_value;
    class util;

    class json_array : public util
    {
    private:
        std::vector<json::json_value> elements;

    public:
        json_array() = default;

        json_array(const json_array &array);

        json_array(const std::string &jsonString);

        ~json_array() = default;

        size_t size() const;

        void add(const std::string &element);

        void add(const char *element);

        void add(const int &element);

        void add(const bool &element);

        void add(const double &element);

        void add(const std::shared_ptr<json_object> &element);

        void add(const std::shared_ptr<json_array> &element);

        template <typename T>
        T get(size_t index) const;

        std::string to_json_string() const override;

        json_array &operator<<(const std::string &element);

        json_array &operator<<(const bool &element);

        json_array &operator<<(const int &element);

        json_array &operator<<(const double &element);
    };
}

#endif