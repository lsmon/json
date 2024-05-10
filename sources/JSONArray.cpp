#include <sstream>
#include "JSONArray.hpp"
#include "JSONParser.hpp"

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

    std::string JSONArray::toJSONString() const
    {
        std::stringstream ss;
        ss << "[";
        size_t i = 0;
        for (auto &element : elements)
        {
            if (auto str = std::get_if<std::string>(&element.getRecord()))
            {
                ss << *str;
            }
            else if (auto integer = std::get_if<int>(&element.getRecord()))
            {
                ss << *integer;
            }
            else if (auto boolean = std::get_if<bool>(&element.getRecord()))
            {
                ss << (*boolean ? "true" : "false");
            }
            else if (auto floating = std::get_if<double>(&element.getRecord()))
            {
                ss << *floating;
            }
            else if (auto obj = std::get_if<std::shared_ptr<JSONObject>>(&element.getRecord()))
            {
                ss << (*obj)->toJSONString();
            }
            else if (auto arr = std::get_if<std::shared_ptr<JSONArray>>(&element.getRecord()))
            {
                ss << (*arr)->toJSONString();
            }
            else
            {
                ss << "null";
            }

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