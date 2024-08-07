#include "json/json_parser.h"

#include <sstream>

namespace Json {

Parser::Parser(std::string_view data) : Parsing::Parser(data) {}

Bool Parser::parse(Value &out_value)
{
    if (Array arr; get_array(arr)) {
        out_value = Value(std::move(arr));
        return true;
    }
    if (Object obj; get_object(obj)) {
        out_value = Value(std::move(obj));
        return true;
    }
    return false;
}

Bool Parser::get_whitespace()
{
    while (!is_eof() && (is_equal(' ') || is_equal('\n') || is_equal('\r') ||
                         is_equal('\t'))) {
        move_next();
    }
    return true;
}

Bool Parser::get_null()
{
    push_save();
    if (require("null")) {
        return true;
    }
    load_save();
    return false;
}

Bool Parser::get_bool(Bool &out_bool)
{
    push_save();
    if (require("true")) {
        out_bool = true;
        pop_save();
        return true;
    }
    load_save(false);
    if (require("false")) {
        out_bool = false;
        pop_save();
        return true;
    }
    load_save();
    return false;
}

Bool Parser::get_exponent(Int &out_exp)
{
    if (!(require('e') || require('E'))) {
        return false;
    }
    push_save();
    Bool neg = false;
    if (require('-')) {
        neg = true;
    } else {
        require('+');  // Consume possible positive
    }
    Nat8 digit;
    if (!get_digit(digit)) {
        load_save();
        return false;
    }
    Int exp = static_cast<Int>(digit) * (neg ? -1 : 1);
    while (get_digit(digit)) {
        exp = exp * 10 + digit;
    }
    out_exp = exp;
    pop_save();
    return true;
}

Bool Parser::get_number(std::variant<Int, Float> &out_number)
{
    push_save();
    Bool neg = require('-');
    Nat8 digit;
    if (!get_digit(digit)) {
        load_save();
        return false;
    }
    Int integral_part = static_cast<Int>(digit) * (neg ? -1 : 1);
    if (digit != 0) {
        while (get_digit(digit)) {
            integral_part = integral_part * 10 + digit;
        }
    }
    bool is_float = false;
    Float fractional_part = 0.0f;
    if (require('.')) {
        is_float = true;
        if (!get_digit(digit)) {
            load_save();
            return false;
        }
        fractional_part = static_cast<Float>(digit) / 10;
        while (get_digit(digit)) {
            fractional_part = (fractional_part + digit) / 10;
        }
    }
    Int exponential_part = 0;
    if (get_exponent(exponential_part)) {
        is_float = true;
    }

    if (is_float) {
        Float number = static_cast<Float>(integral_part);
        number += fractional_part;
        number *= pow(10, exponential_part);
        out_number = number;
    } else {
        out_number = integral_part;
    }
    pop_save();
    return true;
}

Bool Parser::get_escape_char(String &out_char)
{
    push_save();
    if (!require('\\')) {
        pop_save();
        return false;
    }
    if (require('\"')) {
        out_char = '\"';
    } else if (require('\\')) {
        out_char = '\\';
    } else if (require('/')) {
        out_char = '/';
    } else if (require('b')) {
        out_char = '\b';
    } else if (require('f')) {
        out_char = '\f';
    } else if (require('n')) {
        out_char = '\n';
    } else if (require('r')) {
        out_char = '\r';
    } else if (require('t')) {
        out_char = '\t';
    } else if (require('u')) {
        Nat8 hex[4]{};
        if (get_hex_digit(hex[3]) && get_hex_digit(hex[2]) &&
            get_hex_digit(hex[1]) && get_hex_digit(hex[1])) {
            Char32 c = hex[3] << 24 | hex[2] << 16 | hex[1] << 8 | hex[0];
            if (c <= 0x7F) {
                out_char = static_cast<Char>(c);
            } else if (c <= 0x7FF) {
                out_char = std::string(
                    {static_cast<Char>(0xC0 | ((c >> 6) & 0x1F)),
                     static_cast<Char>(0x80 | ((c >> 0) & 0x3F))});
            } else {
                out_char = std::string(
                    {static_cast<Char>(0xE0 | ((c >> 12) & 0x0F)),
                     static_cast<Char>(0x80 | ((c >> 6) & 0x3F)),
                     static_cast<Char>(0x80 | ((c >> 0) & 0x3F))});
            }
        } else {
            load_save();
            return false;
        }
    } else {
        load_save();
        return false;
    }
    pop_save();
    return true;
}

Bool Parser::get_string(String &out_string)
{
    push_save();
    if (!require('\"')) {
        pop_save();
        return false;
    }
    std::stringstream ss;
    std::string escape_char;
    while (!is_eof()) {
        if (require('\"')) {
            out_string = ss.str();
            pop_save();
            return true;
        } else if (get_escape_char(escape_char)) {
            ss << escape_char;
        } else {
            ss << get_current();
            move_next();
        }
    }
    load_save();
    return false;
}

Bool Parser::get_object(Object &out_object)
{
    push_save();
    if (!require('{')) {
        load_save();
        return false;
    }
    out_object = {};
    get_whitespace();
    if (require('}')) {
        pop_save();
        return true;
    }
    String key;
    Value value;
    while (true) {
        get_whitespace();
        if (!get_string(key)) {
            load_save();
            return false;
        }
        get_whitespace();
        if (!require(':')) {
            load_save();
            return false;
        }
        if (!get_value(value)) {
            load_save();
            return false;
        }
        out_object.insert({key, value});
        value = {};
        if (require('}')) {
            pop_save();
            return true;
        }
        if (!require(',')) {
            load_save();
            return false;
        }
    }
    return true;
}

Bool Parser::get_array(Array &out_array)
{
    push_save();
    if (!require('[')) {
        load_save();
        return false;
    }
    out_array = {};
    get_whitespace();
    if (require(']')) {
        pop_save();
        return true;
    }
    Value value;
    while (true) {
        if (!get_value(value)) {
            load_save();
            return false;
        }
        out_array.push_back(value);
        if (require(']')) {
            pop_save();
            return true;
        }
        if (!require(',')) {
            load_save();
            return false;
        }
    }
    return true;
}

Bool Parser::get_value(Value &out_value)
{
    push_save();
    get_whitespace();

    if (get_null()) {
        out_value = Value{};
    } else if (bool b; get_bool(b)) {
        out_value = Value(b);
    } else if (std::variant<Int, Float> num; get_number(num)) {
        if (std::holds_alternative<Int>(num)) {
            out_value = Value(std::get<Int>(num));
        } else {
            out_value = Value(std::get<Float>(num));
        }
    } else if (String str; get_string(str)) {
        out_value = Value(std::move(str));
    } else if (Array arr; get_array(arr)) {
        out_value = Value(std::move(arr));
    } else if (Object obj; get_object(obj)) {
        out_value = Value(std::move(obj));
        return true;
    } else {
        load_save();
        return false;
    }

    get_whitespace();
    pop_save();
    return true;
}
}  // namespace Json