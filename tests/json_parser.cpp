#include "json/json_parser.h"

#include <gtest/gtest.h>

using namespace Json;

TEST(JsonParser, Empty)
{
    Parser parser("{}");
    Value value;
    ASSERT_TRUE(parser.parse(value));
    EXPECT_TRUE(value.is<Object>());
    const Object *obj = value.get_if<Object>();
    EXPECT_NE(obj, nullptr);
    EXPECT_EQ(obj->size(), 0);
}

TEST(JsonParser, PrimativeTypes)
{
    Parser parser(
        R"({"null": null, "true": true, "false": false, "int": 1969,
         "float": 54.2})");
    Value value;
    ASSERT_TRUE(parser.parse(value));
    EXPECT_TRUE(value.is<Object>());
    Object *obj = value.get_if<Object>();
    EXPECT_NE(obj, nullptr);
    EXPECT_EQ(obj->size(), 5);

    EXPECT_TRUE(obj->contains("null"));
    EXPECT_TRUE((*obj)["null"].is<Null>());

    EXPECT_TRUE(obj->contains("true"));
    EXPECT_TRUE((*obj)["true"].is<Bool>());
    EXPECT_TRUE((*obj)["true"].get<Bool>());

    EXPECT_TRUE(obj->contains("false"));
    EXPECT_TRUE((*obj)["false"].is<Bool>());
    EXPECT_FALSE((*obj)["false"].get<Bool>());

    EXPECT_TRUE(obj->contains("int"));
    EXPECT_TRUE((*obj)["int"].is<Int>());
    EXPECT_EQ((*obj)["int"].get<Int>(), 1969);

    EXPECT_TRUE(obj->contains("float"));
    EXPECT_TRUE((*obj)["float"].is<Float>());
    EXPECT_FLOAT_EQ((*obj)["float"].get<Float>(), 54.2f);
}

TEST(JsonParser, Array)
{
    Parser parser(R"([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])");
    Value value;
    ASSERT_TRUE(parser.parse(value));
    EXPECT_TRUE(value.is<Array>());
    Array *arr = value.get_if<Array>();
    EXPECT_NE(arr, nullptr);
    EXPECT_EQ(arr->size(), 10);

    for (int i = 0; i < 10; i++) {
        EXPECT_TRUE((*arr)[i].is<Int>());
        EXPECT_EQ((*arr)[i].get<Int>(), i + 1);
    }
}

TEST(JsonParser, NestedObjects)
{
    Parser parser(R"({"nested":{"child": 12}})");
    Value value;
    ASSERT_TRUE(parser.parse(value));
    EXPECT_TRUE(value.is<Object>());
    Object &obj = value.get<Object>();
    EXPECT_TRUE(obj.contains("nested"));
    EXPECT_TRUE(obj["nested"].is<Object>());
    Object &nested_obj = obj["nested"].get<Object>();
    EXPECT_TRUE(nested_obj.contains("child"));
    EXPECT_TRUE(nested_obj["child"].is<Int>());
    EXPECT_EQ(nested_obj["child"].get<Int>(), 12);
}