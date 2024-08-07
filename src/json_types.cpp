#include "json/json_types.h"

namespace Json {
Value::Value() : data() {}

Value::Value(Object &&obj) : data(obj) {}

Value::Value(Array &&arr) : data(arr) {}

Value::Value(String &&str) : data(str) {}

Value::Value(Float f) : data(f) {}

Value::Value(Int i) : data(i) {}

Value::Value(Bool b) : data(b) {}
}  // namespace Json