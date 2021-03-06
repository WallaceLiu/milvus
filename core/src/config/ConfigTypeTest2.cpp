// Copyright (C) 2019-2020 Zilliz. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under the License.

#include "config/ServerConfig.h"
#include "gtest/gtest.h"

namespace milvus {

#define _MODIFIABLE (true)
#define _IMMUTABLE (false)

template <typename T>
class Utils {
 public:
    static bool
    valid_check_failure(const T& value, std::string& err) {
        err = "Value is invalid.";
        return false;
    }

    static bool
    valid_check_raise_string(const T& value, std::string& err) {
        throw "string exception";
    }

    static bool
    valid_check_raise_exception(const T& value, std::string& err) {
        throw std::bad_alloc();
    }
};

/* BoolConfigTest */
class BoolConfigTest : public testing::Test, public Utils<bool> {};

TEST_F(BoolConfigTest, init_twice_test) {
    Value<bool> bool_value;
    auto bool_config = CreateBoolConfig("b", _MODIFIABLE, bool_value, true, nullptr);
    ASSERT_DEATH(
        {
            bool_config->Init();
            bool_config->Init();
        },
        "initialized");
}

TEST_F(BoolConfigTest, non_init_test) {
    Value<bool> bool_value;
    auto bool_config = CreateBoolConfig("b", _MODIFIABLE, bool_value, true, nullptr);
    ASSERT_DEATH(bool_config->Set("false", true), "uninitialized");
    ASSERT_DEATH(bool_config->Get(), "uninitialized");
}

TEST_F(BoolConfigTest, immutable_update_test) {
    Value<bool> bool_value;
    auto bool_config = CreateBoolConfig("b", _IMMUTABLE, bool_value, true, nullptr);
    bool_config->Init();
    ASSERT_EQ(bool_value(), true);

    EXPECT_THROW(bool_config->Set("false", true), Immutable);
    ASSERT_EQ(bool_value(), true);
}

TEST_F(BoolConfigTest, set_invalid_value_test) {
    Value<bool> bool_value;
    auto bool_config = CreateBoolConfig("b", _MODIFIABLE, bool_value, true, nullptr);
    bool_config->Init();

    EXPECT_THROW(bool_config->Set(" false", true), Invalid);
    ASSERT_EQ(bool_config->Get(), "true");

    EXPECT_THROW(bool_config->Set("false ", true), Invalid);
    ASSERT_EQ(bool_config->Get(), "true");

    EXPECT_THROW(bool_config->Set("afalse", true), Invalid);
    ASSERT_EQ(bool_config->Get(), "true");

    EXPECT_THROW(bool_config->Set("falsee", true), Invalid);
    ASSERT_EQ(bool_config->Get(), "true");

    EXPECT_THROW(bool_config->Set("abcdefg", true), Invalid);
    ASSERT_EQ(bool_config->Get(), "true");

    EXPECT_THROW(bool_config->Set("123456", true), Invalid);
    ASSERT_EQ(bool_config->Get(), "true");

    EXPECT_THROW(bool_config->Set("", true), Invalid);
    ASSERT_EQ(bool_config->Get(), "true");
}

TEST_F(BoolConfigTest, valid_check_fail_test) {
    Value<bool> bool_value;
    auto bool_config = CreateBoolConfig("b", _MODIFIABLE, bool_value, true, valid_check_failure);
    bool_config->Init();

    EXPECT_THROW(bool_config->Set("123456", true), Invalid);
    ASSERT_EQ(bool_config->Get(), "true");
}

TEST_F(BoolConfigTest, string_exception_test) {
    Value<bool> bool_value;
    auto bool_config = CreateBoolConfig("b", _MODIFIABLE, bool_value, true, valid_check_raise_string);
    bool_config->Init();

    EXPECT_THROW(bool_config->Set("false", true), Unexpected);
    ASSERT_EQ(bool_config->Get(), "true");
}

TEST_F(BoolConfigTest, standard_exception_test) {
    Value<bool> bool_value;
    auto bool_config = CreateBoolConfig("b", _MODIFIABLE, bool_value, true, valid_check_raise_exception);
    bool_config->Init();

    EXPECT_THROW(bool_config->Set("false", true), Unexpected);
    ASSERT_EQ(bool_config->Get(), "true");
}

/* StringConfigTest */
class StringConfigTest : public testing::Test, public Utils<std::string> {};

TEST_F(StringConfigTest, init_twice_test) {
    Value<std::string> string_value;
    auto string_config = CreateStringConfig("s", _MODIFIABLE, string_value, "Magic", nullptr);
    ASSERT_DEATH(
        {
            string_config->Init();
            string_config->Init();
        },
        "initialized");
}

TEST_F(StringConfigTest, non_init_test) {
    Value<std::string> string_value;
    auto string_config = CreateStringConfig("s", _MODIFIABLE, string_value, "Magic", nullptr);
    ASSERT_DEATH(string_config->Set("value", true), "uninitialized");
    ASSERT_DEATH(string_config->Get(), "uninitialized");
}

TEST_F(StringConfigTest, immutable_update_test) {
    Value<std::string> string_value;
    auto string_config = CreateStringConfig("s", _IMMUTABLE, string_value, "Magic", nullptr);
    string_config->Init();
    ASSERT_EQ(string_value(), "Magic");

    EXPECT_THROW(string_config->Set("cigaM", true), Immutable);
    ASSERT_EQ(string_value(), "Magic");
}

TEST_F(StringConfigTest, valid_check_fail_test) {
    Value<std::string> string_value;
    auto string_config = CreateStringConfig("s", _MODIFIABLE, string_value, "Magic", valid_check_failure);
    string_config->Init();

    EXPECT_THROW(string_config->Set("123456", true), Invalid);
    ASSERT_EQ(string_config->Get(), "Magic");
}

TEST_F(StringConfigTest, string_exception_test) {
    Value<std::string> string_value;
    auto string_config = CreateStringConfig("s", _MODIFIABLE, string_value, "Magic", valid_check_raise_string);
    string_config->Init();

    EXPECT_THROW(string_config->Set("any", true), Unexpected);
    ASSERT_EQ(string_config->Get(), "Magic");
}

TEST_F(StringConfigTest, standard_exception_test) {
    Value<std::string> string_value;
    auto string_config = CreateStringConfig("s", _MODIFIABLE, string_value, "Magic", valid_check_raise_exception);
    string_config->Init();

    EXPECT_THROW(string_config->Set("any", true), Unexpected);
    ASSERT_EQ(string_config->Get(), "Magic");
}

/* IntegerConfigTest */
class IntegerConfigTest : public testing::Test, public Utils<int64_t> {};

TEST_F(IntegerConfigTest, init_twice_test) {
    Value<int64_t> integer_value;
    auto integer_config = CreateIntegerConfig("i", true, 1024, 65535, integer_value, 19530, nullptr);
    ASSERT_DEATH(
        {
            integer_config->Init();
            integer_config->Init();
        },
        "initialized");
}

TEST_F(IntegerConfigTest, non_init_test) {
    Value<int64_t> integer_value;
    auto integer_config = CreateIntegerConfig("i", true, 1024, 65535, integer_value, 19530, nullptr);
    ASSERT_DEATH(integer_config->Set("42", true), "uninitialized");
    ASSERT_DEATH(integer_config->Get(), "uninitialized");
}

TEST_F(IntegerConfigTest, immutable_update_test) {
    Value<int64_t> integer_value;
    auto integer_config = CreateIntegerConfig("i", _IMMUTABLE, 1024, 65535, integer_value, 19530, nullptr);
    integer_config->Init();
    ASSERT_EQ(integer_value(), 19530);

    EXPECT_THROW(integer_config->Set("2048", true), Immutable);
    ASSERT_EQ(integer_value(), 19530);
}

TEST_F(IntegerConfigTest, set_invalid_value_test) {
}

TEST_F(IntegerConfigTest, valid_check_fail_test) {
    Value<int64_t> integer_value;
    auto integer_config = CreateIntegerConfig("i", true, 1024, 65535, integer_value, 19530, valid_check_failure);
    integer_config->Init();
    EXPECT_THROW(integer_config->Set("2048", true), Invalid);
    ASSERT_EQ(integer_config->Get(), "19530");
}

TEST_F(IntegerConfigTest, string_exception_test) {
    Value<int64_t> integer_value;
    auto integer_config = CreateIntegerConfig("i", true, 1024, 65535, integer_value, 19530, valid_check_raise_string);
    integer_config->Init();

    EXPECT_THROW(integer_config->Set("2048", true), Unexpected);
    ASSERT_EQ(integer_config->Get(), "19530");
}

TEST_F(IntegerConfigTest, standard_exception_test) {
    Value<int64_t> integer_value;
    auto integer_config =
        CreateIntegerConfig("i", true, 1024, 65535, integer_value, 19530, valid_check_raise_exception);
    integer_config->Init();

    EXPECT_THROW(integer_config->Set("2048", true), Unexpected);
    ASSERT_EQ(integer_config->Get(), "19530");
}

TEST_F(IntegerConfigTest, out_of_range_test) {
    Value<int64_t> integer_value;
    auto integer_config = CreateIntegerConfig("i", true, 1024, 65535, integer_value, 19530, nullptr);
    integer_config->Init();

    {
        EXPECT_THROW(integer_config->Set("1023", true), OutOfRange<int64_t>);
        ASSERT_EQ(integer_config->Get(), "19530");
    }

    {
        EXPECT_THROW(integer_config->Set("65536", true), OutOfRange<int64_t>);
        ASSERT_EQ(integer_config->Get(), "19530");
    }
}

TEST_F(IntegerConfigTest, invalid_bound_test) {
    Value<int64_t> integer_value;
    auto integer_config = CreateIntegerConfig("i", true, 100, 0, integer_value, 50, nullptr);
    integer_config->Init();

    EXPECT_THROW(integer_config->Set("30", true), OutOfRange<int64_t>);
    ASSERT_EQ(integer_config->Get(), "50");
}

TEST_F(IntegerConfigTest, invalid_format_test) {
    Value<int64_t> integer_value;
    auto integer_config = CreateIntegerConfig("i", true, 0, 100, integer_value, 50, nullptr);
    integer_config->Init();

    {
        EXPECT_THROW(integer_config->Set("3-0", true), Invalid);
        ASSERT_EQ(integer_config->Get(), "50");
    }

    {
        EXPECT_THROW(integer_config->Set("30-", true), Invalid);
        ASSERT_EQ(integer_config->Get(), "50");
    }

    {
        EXPECT_THROW(integer_config->Set("+30", true), Invalid);
        ASSERT_EQ(integer_config->Get(), "50");
    }

    {
        EXPECT_THROW(integer_config->Set("a30", true), Invalid);
        ASSERT_EQ(integer_config->Get(), "50");
    }

    {
        EXPECT_THROW(integer_config->Set("30a", true), Invalid);
        ASSERT_EQ(integer_config->Get(), "50");
    }

    {
        EXPECT_THROW(integer_config->Set("3a0", true), Invalid);
        ASSERT_EQ(integer_config->Get(), "50");
    }
}

/* FloatingConfigTest */
class FloatingConfigTest : public testing::Test, public Utils<double> {};

TEST_F(FloatingConfigTest, init_twice_test) {
    Value<double> floating_value;
    auto floating_config = CreateFloatingConfig("f", true, 1.0, 9.9, floating_value, 4.5, nullptr);
    ASSERT_DEATH(
        {
            floating_config->Init();
            floating_config->Init();
        },
        "initialized");
}

TEST_F(FloatingConfigTest, non_init_test) {
    Value<double> floating_value;
    auto floating_config = CreateFloatingConfig("f", true, 1.0, 9.9, floating_value, 4.5, nullptr);
    ASSERT_DEATH(floating_config->Set("3.14", true), "uninitialized");
    ASSERT_DEATH(floating_config->Get(), "uninitialized");
}

TEST_F(FloatingConfigTest, immutable_update_test) {
    Value<double> floating_value;
    auto floating_config = CreateFloatingConfig("f", _IMMUTABLE, 1.0, 9.9, floating_value, 4.5, nullptr);
    floating_config->Init();
    ASSERT_FLOAT_EQ(floating_value(), 4.5);

    EXPECT_THROW(floating_config->Set("1.23", true), Immutable);
    ASSERT_FLOAT_EQ(std::stof(floating_config->Get()), 4.5);
}

TEST_F(FloatingConfigTest, set_invalid_value_test) {
}

TEST_F(FloatingConfigTest, valid_check_fail_test) {
    Value<double> floating_value;
    auto floating_config = CreateFloatingConfig("f", true, 1.0, 9.9, floating_value, 4.5, valid_check_failure);
    floating_config->Init();

    EXPECT_THROW(floating_config->Set("1.23", true), Invalid);
    ASSERT_FLOAT_EQ(std::stof(floating_config->Get()), 4.5);
}

TEST_F(FloatingConfigTest, string_exception_test) {
    Value<double> floating_value;
    auto floating_config = CreateFloatingConfig("f", true, 1.0, 9.9, floating_value, 4.5, valid_check_raise_string);
    floating_config->Init();

    EXPECT_THROW(floating_config->Set("1.23", true), Unexpected);
    ASSERT_FLOAT_EQ(std::stof(floating_config->Get()), 4.5);
}

TEST_F(FloatingConfigTest, standard_exception_test) {
    Value<double> floating_value;
    auto floating_config = CreateFloatingConfig("f", true, 1.0, 9.9, floating_value, 4.5, valid_check_raise_exception);
    floating_config->Init();

    EXPECT_THROW(floating_config->Set("1.23", true), Unexpected);
    ASSERT_FLOAT_EQ(std::stof(floating_config->Get()), 4.5);
}

TEST_F(FloatingConfigTest, out_of_range_test) {
    Value<double> floating_value;
    auto floating_config = CreateFloatingConfig("f", true, 1.0, 9.9, floating_value, 4.5, valid_check_raise_exception);
    floating_config->Init();

    {
        EXPECT_THROW(floating_config->Set("0.99", true), OutOfRange<double>);
        ASSERT_FLOAT_EQ(std::stof(floating_config->Get()), 4.5);
    }

    {
        EXPECT_THROW(floating_config->Set("10.00", true), OutOfRange<double>);
        ASSERT_FLOAT_EQ(std::stof(floating_config->Get()), 4.5);
    }
}

TEST_F(FloatingConfigTest, invalid_bound_test) {
    Value<double> floating_value;
    auto floating_config = CreateFloatingConfig("f", true, 9.9, 1.0, floating_value, 4.5, valid_check_raise_exception);
    floating_config->Init();

    EXPECT_THROW(floating_config->Set("6.0", true), OutOfRange<double>);
    ASSERT_FLOAT_EQ(std::stof(floating_config->Get()), 4.5);
}

TEST_F(FloatingConfigTest, DISABLED_invalid_format_test) {
    Value<double> floating_value;
    auto floating_config = CreateFloatingConfig("f", true, 1.0, 100.0, floating_value, 4.5, nullptr);
    floating_config->Init();

    {
        EXPECT_THROW(floating_config->Set("6.0.1", true), Invalid);
        ASSERT_FLOAT_EQ(std::stof(floating_config->Get()), 4.5);
    }

    {
        EXPECT_THROW(floating_config->Set("6a0", true), Invalid);
        ASSERT_FLOAT_EQ(std::stof(floating_config->Get()), 4.5);
    }
}

/* EnumConfigTest */
class EnumConfigTest : public testing::Test, public Utils<int64_t> {};

TEST_F(EnumConfigTest, init_twice_test) {
    configEnum testEnum{
        {"e", 1},
    };
    Value<int64_t> enum_value;
    auto enum_config = CreateEnumConfig("e", _MODIFIABLE, &testEnum, enum_value, 2, nullptr);
    ASSERT_DEATH(
        {
            enum_config->Init();
            enum_config->Init();
        },
        "initialized");
}

TEST_F(EnumConfigTest, non_init_test) {
    configEnum testEnum{
        {"e", 1},
    };
    Value<int64_t> enum_value;
    auto enum_config = CreateEnumConfig("e", _MODIFIABLE, &testEnum, enum_value, 2, nullptr);
    ASSERT_DEATH(enum_config->Set("e", true), "uninitialized");
    ASSERT_DEATH(enum_config->Get(), "uninitialized");
}

TEST_F(EnumConfigTest, immutable_update_test) {
    configEnum testEnum{
        {"a", 1},
        {"b", 2},
        {"c", 3},
    };
    Value<int64_t> enum_value;
    auto enum_config = CreateEnumConfig("e", _IMMUTABLE, &testEnum, enum_value, 1, nullptr);
    enum_config->Init();
    ASSERT_EQ(enum_value(), 1);

    EXPECT_THROW(enum_config->Set("b", true), Immutable);
    ASSERT_EQ(enum_value(), 1);
}

TEST_F(EnumConfigTest, set_invalid_value_check) {
    configEnum testEnum{
        {"a", 1},
    };
    Value<int64_t> enum_value;
    auto enum_config = CreateEnumConfig("e", _MODIFIABLE, &testEnum, enum_value, 1, nullptr);
    enum_config->Init();

    EXPECT_THROW(enum_config->Set("b", true), EnumValueNotFound);
    ASSERT_EQ(enum_config->Get(), "a");
}

TEST_F(EnumConfigTest, empty_enum_test) {
    configEnum testEnum{};
    Value<int64_t> enum_value;
    auto enum_config = CreateEnumConfig("e", _MODIFIABLE, &testEnum, enum_value, 2, nullptr);
    ASSERT_DEATH(enum_config->Init(), "empty");
}

TEST_F(EnumConfigTest, valid_check_fail_test) {
    configEnum testEnum{
        {"a", 1},
        {"b", 2},
        {"c", 3},
    };
    Value<int64_t> enum_value;
    auto enum_config = CreateEnumConfig("e", _MODIFIABLE, &testEnum, enum_value, 1, valid_check_failure);
    enum_config->Init();

    EXPECT_THROW(enum_config->Set("b", true), Invalid);
    ASSERT_EQ(enum_config->Get(), "a");
}

TEST_F(EnumConfigTest, string_exception_test) {
    configEnum testEnum{
        {"a", 1},
        {"b", 2},
        {"c", 3},
    };
    Value<int64_t> enum_value;
    auto enum_config = CreateEnumConfig("e", _MODIFIABLE, &testEnum, enum_value, 1, valid_check_raise_string);
    enum_config->Init();

    EXPECT_THROW(enum_config->Set("b", true), Unexpected);
    ASSERT_EQ(enum_config->Get(), "a");
}

TEST_F(EnumConfigTest, standard_exception_test) {
    configEnum testEnum{
        {"a", 1},
        {"b", 2},
        {"c", 3},
    };
    Value<int64_t> enum_value;
    auto enum_config = CreateEnumConfig("e", _MODIFIABLE, &testEnum, enum_value, 1, valid_check_raise_exception);
    enum_config->Init();

    EXPECT_THROW(enum_config->Set("b", true), Unexpected);
    ASSERT_EQ(enum_config->Get(), "a");
}

/* SizeConfigTest */
class SizeConfigTest : public testing::Test, public Utils<int64_t> {};

TEST_F(SizeConfigTest, init_twice_test) {
    Value<int64_t> size_value;
    auto size_config = CreateSizeConfig("i", true, 1024, 4096, size_value, 2048, nullptr);
    ASSERT_DEATH(
        {
            size_config->Init();
            size_config->Init();
        },
        "initialized");
}

TEST_F(SizeConfigTest, non_init_test) {
    Value<int64_t> size_value;
    auto size_config = CreateSizeConfig("i", true, 1024, 4096, size_value, 2048, nullptr);
    ASSERT_DEATH(size_config->Set("3000", true), "uninitialized");
    ASSERT_DEATH(size_config->Get(), "uninitialized");
}

TEST_F(SizeConfigTest, immutable_update_test) {
    Value<int64_t> size_value;
    auto size_config = CreateSizeConfig("i", _IMMUTABLE, 1024, 4096, size_value, 2048, nullptr);
    size_config->Init();
    ASSERT_EQ(size_value(), 2048);

    EXPECT_THROW(size_config->Set("3000", true), Immutable);
    ASSERT_EQ(size_value(), 2048);
}

TEST_F(SizeConfigTest, set_invalid_value_test) {
}

TEST_F(SizeConfigTest, valid_check_fail_test) {
    Value<int64_t> size_value;
    auto size_config = CreateSizeConfig("i", true, 1024, 4096, size_value, 2048, valid_check_failure);
    size_config->Init();

    EXPECT_THROW(size_config->Set("3000", true), Invalid);
    ASSERT_EQ(size_config->Get(), "2KB");
}

TEST_F(SizeConfigTest, string_exception_test) {
    Value<int64_t> size_value;
    auto size_config = CreateSizeConfig("i", true, 1024, 4096, size_value, 2048, valid_check_raise_string);
    size_config->Init();

    EXPECT_THROW(size_config->Set("3000", true), Unexpected);
    ASSERT_EQ(size_config->Get(), "2KB");
}

TEST_F(SizeConfigTest, standard_exception_test) {
    Value<int64_t> size_value;
    auto size_config = CreateSizeConfig("i", true, 1024, 4096, size_value, 2048, valid_check_raise_exception);
    size_config->Init();

    EXPECT_THROW(size_config->Set("3000", true), Unexpected);
    ASSERT_EQ(size_config->Get(), "2KB");
}

TEST_F(SizeConfigTest, out_of_range_test) {
    Value<int64_t> size_value;
    auto size_config = CreateSizeConfig("i", true, 1024, 4096, size_value, 2048, nullptr);
    size_config->Init();

    {
        EXPECT_THROW(size_config->Set("1023", true), OutOfRange<int64_t>);
        ASSERT_EQ(size_config->Get(), "2KB");
    }

    {
        EXPECT_THROW(size_config->Set("4097", true), OutOfRange<int64_t>);
        ASSERT_EQ(size_config->Get(), "2KB");
    }
}

TEST_F(SizeConfigTest, negative_integer_test) {
    Value<int64_t> size_value;
    auto size_config = CreateSizeConfig("i", true, 1024, 4096, size_value, 2048, nullptr);
    size_config->Init();
    EXPECT_THROW(size_config->Set("-3KB", true), Invalid);
    ASSERT_EQ(size_config->Get(), "2KB");
}

TEST_F(SizeConfigTest, invalid_bound_test) {
    Value<int64_t> size_value;
    auto size_config = CreateSizeConfig("i", true, 100, 0, size_value, 50, nullptr);
    size_config->Init();

    EXPECT_THROW(size_config->Set("30", true), OutOfRange<int64_t>);
    ASSERT_EQ(size_config->Get(), "50");
}

TEST_F(SizeConfigTest, invalid_unit_test) {
    Value<int64_t> size_value;
    auto size_config = CreateSizeConfig("i", true, 1024, 4096, size_value, 2048, nullptr);
    size_config->Init();

    EXPECT_THROW(size_config->Set("1 TB", true), Invalid);
    ASSERT_EQ(size_config->Get(), "2KB");
}

TEST_F(SizeConfigTest, invalid_format_test) {
    Value<int64_t> size_value;
    auto size_config = CreateSizeConfig("i", true, 1024, 4096, size_value, 2048, nullptr);
    size_config->Init();

    {
        EXPECT_THROW(size_config->Set("a10GB", true), Invalid);
        ASSERT_EQ(size_config->Get(), "2KB");
    }

    {
        EXPECT_THROW(size_config->Set("200*0", true), Invalid);
        ASSERT_EQ(size_config->Get(), "2KB");
    }

    {
        EXPECT_THROW(size_config->Set("10AB", true), Invalid);
        ASSERT_EQ(size_config->Get(), "2KB");
    }
}

}  // namespace milvus
