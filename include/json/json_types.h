#pragma once

#include <types/types.h>

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace Json {

class Value;

using Null = std::monostate;
using String = std::string;
using Array = std::vector<Value>;
using Object = std::map<String, Value, std::less<>>;

class Value {
  public:
    Value();
    Value(Object &&obj);
    Value(Array &&arr);
    Value(String &&str);
    Value(Float f);
    Value(Int i);
    Value(Bool b);

    template <typename T>
    inline bool is() const;

    template <typename T>
    inline T &get();

    template <typename T>
    inline T *get_if();

  private:
    std::variant<Null, Bool, Int, Float, String, Array, Object> data;

  private:
    template <typename T, typename... Ts>
    static constexpr bool is_in_parameter_pack(const std::variant<Ts...> &)
    {
        return (std::is_same_v<T, Ts> || ...);
    }

    template <typename T>
    static constexpr bool is_value_type =
        is_in_parameter_pack<T>(decltype(data)());
};
}  // namespace Json

namespace Json {
template <typename T>
inline bool Value::is() const
{
    static_assert(is_value_type<T>, "Type must be valid JSON value type");
    if constexpr (is_value_type<T>) {
        return std::holds_alternative<T>(this->data);
    }
}

template <typename T>
inline T &Value::get()
{
    static_assert(is_value_type<T>, "Type must be valid JSON value type");
    if constexpr (is_value_type<T>) {
        return std::get<T>(this->data);
    }
}

template <typename T>
inline T *Value::get_if()
{
    static_assert(is_value_type<T>, "Type must be valid JSON value type");
    if constexpr (is_value_type<T>) {
        return std::get_if<T>(&this->data);
    }
}
}  // namespace Json