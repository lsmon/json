#include "json_parser.hpp"
#include <stack>
#include <iostream>


namespace json {
    json_array parser::parse_array(std::string_view jsonView)
    {
        if (!util::validate(jsonView)) {
            throw std::runtime_error("json string square braces not balanced");
        }
        if (jsonView.empty()) {
            throw std::runtime_error("json string is empty");
        }
        size_t pos = 0;
        jsonView = util::trim(jsonView);
        if (!(jsonView[pos] == OSB)) {
            throw std::runtime_error("json array needs to start with square brackets '[]' or curly braces '{}' ");
        }
        json_array jsonArray;
        do {
            switch (jsonView[pos]) {
                case '[': {
                    jsonView = strip_square_brackets(jsonView);
                    std::vector<std::string_view> splits = split_string_view_by(jsonView, ',');
                    for(auto s : splits) {
                        if (s[0] == OCB) {
                            jsonArray.add(std::make_shared<json_object>(parse_object(s)));
                        } else if (s[0] == OSB) {
                            jsonArray.add(std::make_shared<json_array>(parse_array(s)));
                        } else {
                            std::string_view element = trim(s);
                            if (is_boolean(element)) {
                                jsonArray.add((element == "true" || element == "True"));
                            } else if (is_integer(element)) {
                                jsonArray.add(to_integer(element));
                            } else if (is_double(element)) {
                                jsonArray.add(to_double(element));
                            } else {
                                jsonArray.add(std::string(element));
                            }
                        }
                    }
                    pos += jsonView.size();
                }
                break;
            }
            if (pos <= jsonView.size()) pos++;
        } while (pos <= jsonView.size() && !((jsonView[pos] != CCB || jsonView[pos] != CSB)));

        return jsonArray;
    }

    json_object parser::parse_object(std::string_view jsonView) {
        json_object jsonObj;
        if (!util::validate(jsonView)) {
            throw std::runtime_error("json string brackets not balanced");
        }
        if (jsonView.empty()) {
            throw std::runtime_error("json string is empty");
        }
        size_t pos = 0;
        jsonView = util::trim(jsonView);
        if (!(jsonView[pos] == OCB || jsonView[pos] == OSB)) {
            throw std::runtime_error("json string needs to start with square brackets '[' or curly braces '{' ");
        }
        do {
            switch (jsonView[pos]) {
                case '{': {
                    jsonView = strip_curly_braces(jsonView);
                    std::vector<std::string_view> splits = split_string_view_by(jsonView, ',');

                    for (std::string_view obj : splits) {
                        obj = trim(obj);
                        std::string_view key = trim(obj.substr(0, obj.find_first_of(':')));
                        std::string_view value = trim(obj.substr(obj.find_first_of(':') + 1)); 
                        if (value[0] == OCB) {
                            jsonObj.put(std::string(key), std::make_shared<json_object>(parse_object(value)));
                        } else if (value[0] == OSB) {
                            jsonObj.put(std::string(key), std::make_shared<json_array>(parse_array(value)));
                        } else {
                            value = util::trim(value);
                            if (is_boolean(value)) {
                                jsonObj.put(std::string(key), (value == "true" || value == "True"));
                            } else if (is_integer(value)) {
                                jsonObj.put(std::string(key), to_integer(value));
                            } else if (is_double(value)) {
                                jsonObj.put(std::string(key), to_double(value)); 
                            } else {
                                jsonObj.put(std::string(key), std::string(value));
                            }
                        }
                        pos += obj.size();
                    }
                }
                break;
            }
            if (pos <= jsonView.size()) pos++;
        } while (pos <= jsonView.size() && !((jsonView[pos] != CCB || jsonView[pos] != CSB)));
        return jsonObj;
    }

    bool parser::validate(const std::string &jsonString)
    {
        std::string_view stringView(jsonString);
        return util::validate(stringView);
    }
}