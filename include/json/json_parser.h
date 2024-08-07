#pragma once

#include "json/json_types.h"

#include <parser/parser.h>

namespace Json {
class Parser : public Parsing::Parser {
  public:
    Parser(std::string_view data);

    Bool parse(Value &out_value);

  protected:
    Bool get_whitespace();
    Bool get_null();
    Bool get_bool(Bool &out_bool);
    Bool get_exponent(Int &out_exp);
    Bool get_number(std::variant<Int, Float> &out_number);
    Bool get_escape_char(String &out_char);
    Bool get_string(String &out_string);
    Bool get_object(Object &out_object);
    Bool get_array(Array &out_array);
    Bool get_value(Value &out_value);
};
}  // namespace Json