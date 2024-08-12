#include "json/json_types.h"

#include <gtest/gtest.h>

using namespace Json;

TEST(JsonTypes, NullValueToString)
{
    Value value;
    EXPECT_EQ(value.to_string(), "null");
}

TEST(JsonTypes, BoolValueToString)
{
    Value value(true);
    EXPECT_EQ(value.to_string(), "true");
    value = Value(false);
    EXPECT_EQ(value.to_string(), "false");
}

TEST(JsonTypes, IntValueToString)
{
    Value value(1969);
    EXPECT_EQ(value.to_string(), "1969");
}

TEST(JsonTypes, FloatValueToString)
{
    Value value(20.5f);
    EXPECT_EQ(value.to_string(), "20.5");
}

TEST(JsonTypes, StringValueToString)
{
    Value value("hello world");
    EXPECT_EQ(value.to_string(), "\"hello world\"");
}

TEST(JsonTypes, ArrayValueToString)
{
    Value value(Array{1, 2, 3, 4, 5});
    EXPECT_EQ(value.to_string(), "[1,2,3,4,5]");
}

TEST(JsonTypes, ObjectValueToString)
{
    Value value(Object{{"hello", "world"}, {"num", 5}});
    EXPECT_EQ(value.to_string(), "{\"hello\":\"world\",\"num\":5}");
}