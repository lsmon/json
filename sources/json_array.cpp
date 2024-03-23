#include <sstream>
#include "json_array.hpp"

namespace json {

    void json_array::add(const std::string &element) {
        elements.push_back(std::move(element));
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

    void json_array::add(const json_object& element) {
        elements.push_back(element);
    }

    void json_array::add(const json_array &element){
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
            if (std::holds_alternative<int>(element)) {
                ss << std::get<int>(element);
            } else if (std::holds_alternative<double>(element)) {
                ss << std::get<double>(element);
            } else if (std::holds_alternative<bool>(element)) {
                ss << (std::get<bool>(element)?"true":"false");
            } else if (std::holds_alternative<std::string>(element)) {
                ss << std::get<std::string>(element);
            } else if (std::holds_alternative<json_object>(element)) {
                ss << std::get<json_object>(element).to_json_string();
            } else if (std::holds_alternative<json_array>(element)) {
                ss << std::get<json_array>(element).to_json_string();
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