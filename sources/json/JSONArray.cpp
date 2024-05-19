#include <sstream>
#include "json/JSONArray.hpp"
#include "json/JSONParser.hpp"

namespace json
{
    JSONArray::JSONArray(const JSONArray &array)
    {
        for (const auto &element : array.elements)
        {
            //            if (auto obj = std::get_if<std::shared_ptr<json_object>>(&element.getRecord())) {
            elements.push_back(element);
            //            } else if (auto arr = std::get_if<std::shared_ptr<json_array>>(&element.getRecord())) {
            //                elements.push_back(element);
            //            } else {
            //                elements.push_back(element);
            //            }
        }
    }

    JSONArray::JSONArray(const std::string &jsonString)
    {
        auto parsed = Parser::parse(jsonString);
        if (parsed)
        {
            auto arr = dynamic_cast<JSONArray *>(parsed.get());
            if (arr)
            {
                *this = std::move(*arr);
            }
            else
            {
                throw std::runtime_error("Parsed string is not a json_array");
            }
        }
        else
        {
            throw std::runtime_error("JSON String parsing failed.");
        }
    }

    size_t JSONArray::size() const
    {
        return elements.size();
    }

    void JSONArray::add(const std::string &element)
    {
        elements.emplace_back(element);
    }

    void JSONArray::add(const char *element)
    {
        elements.emplace_back(std::move(std::string(element)));
    }

    void JSONArray::add(const int &element)
    {
        elements.emplace_back(element);
    }

    void JSONArray::add(const bool &element)
    {
        elements.emplace_back(element);
    }

    void JSONArray::add(const double &element)
    {
        elements.emplace_back(element);
    }

    void JSONArray::add(const std::shared_ptr<JSONObject> &element)
    {
        elements.emplace_back(element);
    }

    void JSONArray::add(const std::shared_ptr<JSONArray> &element)
    {
        elements.emplace_back(element);
    }
    template <typename T>
    T JSONArray::get(size_t index) const
    {
        return elements.at(index);
    }

    /// @brief Using visit Method:

    //     The visit method is called on element.getRecord(). It takes a lambda function that handles different types stored in the Variant.
    // Using decltype and std::decay_t:

    // Within the lambda, decltype(value) and std::decay_t are used to deduce the type of the stored value.
    // if constexpr for Compile-time Type Checking:

    // The if constexpr construct is used to perform compile-time type checks. This ensures that the correct code block is executed based on the type of the stored value.
    // Handling Different Types:

    // Each type (std::string, int, bool, double, std::shared_ptr<JSONObject>, std::shared_ptr<JSONArray>) is handled separately within the lambda, similar to how it was handled with std::get_if.
    // Default Case for Unsupported Types:

    // The else block handles any types not explicitly checked, outputting "null".
    // This approach leverages the custom Variant class's visitation mechanism to replace std::get_if and maintain type safety and clarity.
    /// @return std::string JOSNArray string representation inline
    std::string JSONArray::toJSONString() const
    {
        std::stringstream ss;
        ss << "[";
        size_t i = 0;
        for (auto &element : elements)
        {
            element.getRecord().visit([&ss](auto &&value)
            {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<T, std::string>)
                {
                    ss << value;
                }
                else if constexpr (std::is_same_v<T, int>)
                {
                    ss << value;
                }
                else if constexpr (std::is_same_v<T, bool>)
                {
                    ss << (value ? "true" : "false");
                }
                else if constexpr (std::is_same_v<T, double>)
                {
                    ss << value;
                }
                else if constexpr (std::is_same_v<T, std::shared_ptr<JSONObject>>)
                {
                    ss << value->toJSONString();
                }
                else if constexpr (std::is_same_v<T, std::shared_ptr<JSONArray>>)
                {
                    ss << value->toJSONString();
                }
                else
                {
                    ss << "null";
                }
            });

            if (i++ < elements.size() - 1)
            {
                ss << ",";
            }
        }
        ss << "]";
        return ss.str();
    }

    JSONArray &JSONArray::operator<<(const std::string &element)
    {
        add(element);
        return *this;
    }

    JSONArray &JSONArray::operator<<(const bool &element)
    {
        add(element);
        return *this;
    }

    JSONArray &JSONArray::operator<<(const int &element)
    {
        add(element);
        return *this;
    }

    JSONArray &JSONArray::operator<<(const double &element)
    {
        add(element);
        return *this;
    }

}