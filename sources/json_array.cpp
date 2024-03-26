#include <sstream>
#include "json_array.hpp"

namespace json {
    json_array::json_array(const json_array &array)
    {
        for (const auto& element : array.elements) {
            if (auto obj = std::get_if<std::shared_ptr<json_object>>(&element)) {
                elements.push_back(std::make_shared<json_object>(**obj));
            } else if (auto arr = std::get_if<std::shared_ptr<json_array>>(&element)) {
                elements.push_back(std::make_shared<json_array>(**arr));
            } else {
                elements.push_back(element);
            }
        }
    }

    void json_array::add(const std::string &element) {
        elements.push_back(std::move(element));
    }

    void json_array::add(const char *element)
    {
            elements.push_back(std::move(std::string(element)));
    }

    void json_array::add(const int &element) {
        elements.push_back(element);
    }
    
    void json_array::add(const bool &element) {
        elements.push_back(element);
    }
    
    void json_array::add(const double &element) {
        elements.push_back(element);
    }

    void json_array::add(const std::shared_ptr<json_object>& element) {
        elements.push_back(element);
    }

    void json_array::add(const std::shared_ptr<json_array>& element){
        elements.push_back(element);
    }

    template <typename T>
    inline T json_array::get(size_t index) const {
        return elements.at(index);
    }

    std::string json_array::to_json_string() const {
        std::stringstream ss;
        ss << "[";
        size_t i = 0;
        for (auto &element : elements) {
            if (auto str = std::get_if<std::string>(&element)) {
                ss << "\"" << *str << "\"";
            } else if (auto integer = std::get_if<int>(&element)) {
                ss << *integer;
            } else if (auto boolean = std::get_if<bool>(&element)) {
                ss << (*boolean ? "true" : "false");
            } else if (auto floating = std::get_if<double>(&element)) {
                ss << *floating;
            } else if (auto obj = std::get_if<std::shared_ptr<json_object>>(&element)) {
                ss << (*obj)->to_json_string();
            } else if (auto arr = std::get_if<std::shared_ptr<json_array>>(&element)) {
                ss << (*arr)->to_json_string();
            } else {
                ss << "null";
            }
            
            if (i++ < elements.size()-1) {
                ss << ",";
            }
        }
        ss << "]";
        return ss.str();
    }
    
    json_array& json_array::operator <<(const std::string &element) {
        add(element);
        return *this;
    }
    
    json_array& json_array::operator <<(const bool &element) {
        add(element);
        return *this;
    }
    
    json_array& json_array::operator <<(const int &element) {
        add(element);
        return *this;
    }
    
    json_array& json_array::operator <<(const double &element) {
        add(element);
        return *this;
    }

    
} 