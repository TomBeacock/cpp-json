#include "json/json_types.h"

#include <format>
#include <sstream>

namespace Json {
Value::Value() : data() {}

Value::Value(Object &&obj) : data(obj) {}

Value::Value(Array &&arr) : data(arr) {}

Value::Value(String &&str) : data(str) {}

Value::Value(std::string_view str) : data(String(str)) {}

Value::Value(const Char *str) : data(String(str)) {}

Value::Value(Float f) : data(f) {}

Value::Value(Int i) : data(i) {}

Value::Value(Bool b) : data(b) {}

std::string Value::to_string() const
{
    return std::visit(
        [](const auto &data) -> std::string {
            using T = std::decay_t<decltype(data)>;
            if constexpr (std::is_same_v<std::monostate, T>) {
                return "null";
            } else if constexpr (std::is_same_v<Bool, T>) {
                return data ? "true" : "false";
            } else if constexpr (std::is_same_v<Int, T>) {
                return std::format("{}", data);
            } else if constexpr (std::is_same_v<Float, T>) {
                return std::format("{}", data);
            } else if constexpr (std::is_same_v<String, T>) {
                return std::format("\"{}\"", data);
            } else if constexpr (std::is_same_v<Array, T>) {
                std::stringstream ss;
                ss << '[';
                const Char *sep = "";
                for (const Value &value : data) {
                    ss << std::format("{}{}", sep, value.to_string());
                    sep = ",";
                }
                ss << ']';
                return ss.str();
            } else if constexpr (std::is_same_v<Object, T>) {
                std::stringstream ss;
                ss << '{';
                const Char *sep = "";
                for (const auto &[key, value] : data) {
                    ss << std::format(
                        "{}\"{}\":{}", sep, key, value.to_string());
                    sep = ",";
                }
                ss << '}';
                return ss.str();
            }
            return "";
        },
        this->data);
}
}  // namespace Json