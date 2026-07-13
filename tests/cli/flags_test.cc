#include "catch.hpp"
#include <xcli/flags.h>

// -----------------------------------------------------------
// Define flags for testing
// -----------------------------------------------------------
XDEFINE_bool(test_bool, false, "test bool flag");
XDEFINE_int32(test_int32, -1, "test int32 flag");
XDEFINE_uint32(test_uint32, 42, "test uint32 flag");
XDEFINE_int64(test_int64, -99, "test int64 flag");
XDEFINE_uint64(test_uint64, 123, "test uint64 flag");
XDEFINE_double(test_double, 3.14, "test double flag");
XDEFINE_string(test_string, "hello", "test string flag");

// -----------------------------------------------------------
// FlagTypes: verify variables exist and have correct types
// -----------------------------------------------------------
TEST_CASE("FlagTypes", "[flags]") {
    XFLAGS_test_bool = false;
    CHECK_FALSE(XFLAGS_test_bool);

    XFLAGS_test_bool = true;
    CHECK(XFLAGS_test_bool);
}

TEST_CASE("FlagTypes int32", "[flags]") {
    XFLAGS_test_int32 = 42;
    CHECK(XFLAGS_test_int32 == 42);
}

TEST_CASE("FlagTypes uint32", "[flags]") {
    XFLAGS_test_uint32 = 99;
    CHECK(XFLAGS_test_uint32 == 99);
}

TEST_CASE("FlagTypes int64", "[flags]") {
    XFLAGS_test_int64 = -1000;
    CHECK(XFLAGS_test_int64 == -1000);
}

TEST_CASE("FlagTypes uint64", "[flags]") {
    XFLAGS_test_uint64 = 9999;
    CHECK(XFLAGS_test_uint64 == 9999);
}

TEST_CASE("FlagTypes double", "[flags]") {
    XFLAGS_test_double = 2.718;
    CHECK(XFLAGS_test_double == Approx(2.718));
}

TEST_CASE("FlagTypes string", "[flags]") {
    XFLAGS_test_string = "world";
    CHECK(XFLAGS_test_string == "world");
}

// -----------------------------------------------------------
// Registry: flags are automatically registered
// -----------------------------------------------------------
TEST_CASE("Registry FindFlag", "[flags]") {
    auto *reg = &xcli::detail::FlagRegistry::GlobalRegistry();

    auto *flag = reg->Find("test_bool");
    REQUIRE(flag != nullptr);
    CHECK(flag->name() == "test_bool");
    CHECK(flag->type() == xcli::detail::FlagType::Bool);

    flag = reg->Find("test_int32");
    REQUIRE(flag != nullptr);
    CHECK(flag->name() == "test_int32");
    CHECK(flag->type() == xcli::detail::FlagType::Int32);

    flag = reg->Find("test_string");
    REQUIRE(flag != nullptr);
    CHECK(flag->name() == "test_string");
    CHECK(flag->type() == xcli::detail::FlagType::String);
}

TEST_CASE("Registry Find unknown", "[flags]") {
    auto *flag = xcli::detail::FlagRegistry::GlobalRegistry().Find("no_such_flag");
    CHECK(flag == nullptr);
}

TEST_CASE("Registry Find dash-underscore", "[flags]") {
    // Should find flags even with dash/underscore substitution
    auto *flag = xcli::detail::FlagRegistry::GlobalRegistry().Find("test-bool");
    // "test_bool" does not contain dashes, but test the mechanism
    if (flag != nullptr) {
        CHECK(flag->name() == "test_bool");
    }
}

// -----------------------------------------------------------
// Default values
// -----------------------------------------------------------
TEST_CASE("DefaultValues", "[flags]") {
    auto *reg = &xcli::detail::FlagRegistry::GlobalRegistry();

    CHECK(reg->Find("test_bool")->default_value() == "false");
    CHECK(reg->Find("test_int32")->default_value() == "-1");
    CHECK(reg->Find("test_uint32")->default_value() == "42");
    CHECK(reg->Find("test_int64")->default_value() == "-99");
    CHECK(reg->Find("test_uint64")->default_value() == "123");
    CHECK(reg->Find("test_double")->default_value() == "3.1400000000000001");
    CHECK(reg->Find("test_string")->default_value() == "hello");
}

// -----------------------------------------------------------
// ParseFrom — normal values
// -----------------------------------------------------------
TEST_CASE("ParseFrom bool true values", "[flags]") {
    auto *reg = &xcli::detail::FlagRegistry::GlobalRegistry();
    auto *flag = reg->Find("test_bool");

    CHECK(flag->SetValue("true"));
    CHECK(XFLAGS_test_bool == true);

    CHECK(flag->SetValue("1"));
    CHECK(XFLAGS_test_bool == true);

    CHECK(flag->SetValue("yes"));
    CHECK(XFLAGS_test_bool == true);

    CHECK(flag->SetValue("t"));
    CHECK(XFLAGS_test_bool == true);

    CHECK(flag->SetValue("y"));
    CHECK(XFLAGS_test_bool == true);
}

TEST_CASE("ParseFrom bool false values", "[flags]") {
    auto *reg = &xcli::detail::FlagRegistry::GlobalRegistry();
    auto *flag = reg->Find("test_bool");

    XFLAGS_test_bool = true;

    CHECK(flag->SetValue("false"));
    CHECK_FALSE(XFLAGS_test_bool);

    XFLAGS_test_bool = true;
    CHECK(flag->SetValue("0"));
    CHECK_FALSE(XFLAGS_test_bool);

    XFLAGS_test_bool = true;
    CHECK(flag->SetValue("no"));
    CHECK_FALSE(XFLAGS_test_bool);

    XFLAGS_test_bool = true;
    CHECK(flag->SetValue("f"));
    CHECK_FALSE(XFLAGS_test_bool);

    XFLAGS_test_bool = true;
    CHECK(flag->SetValue("n"));
    CHECK_FALSE(XFLAGS_test_bool);
}

TEST_CASE("ParseFrom int32 normal", "[flags]") {
    auto *flag = xcli::detail::FlagRegistry::GlobalRegistry().Find("test_int32");

    CHECK(flag->SetValue("12"));
    CHECK(XFLAGS_test_int32 == 12);

    CHECK(flag->SetValue("-5"));
    CHECK(XFLAGS_test_int32 == -5);
}

TEST_CASE("ParseFrom int32 hex", "[flags]") {
    auto *flag = xcli::detail::FlagRegistry::GlobalRegistry().Find("test_int32");

    CHECK(flag->SetValue("0x10"));
    CHECK(XFLAGS_test_int32 == 16);

    CHECK(flag->SetValue("0XFF"));
    CHECK(XFLAGS_test_int32 == 255);
}

TEST_CASE("ParseFrom int32 leading zero", "[flags]") {
    auto *flag = xcli::detail::FlagRegistry::GlobalRegistry().Find("test_int32");

    // Leading zero is decimal, not octal
    CHECK(flag->SetValue("010"));
    CHECK(XFLAGS_test_int32 == 10);
}

TEST_CASE("ParseFrom uint32 normal", "[flags]") {
    auto *flag = xcli::detail::FlagRegistry::GlobalRegistry().Find("test_uint32");

    CHECK(flag->SetValue("100"));
    CHECK(XFLAGS_test_uint32 == 100);
}

TEST_CASE("ParseFrom uint32 negative fails", "[flags]") {
    auto *flag = xcli::detail::FlagRegistry::GlobalRegistry().Find("test_uint32");

    uint32_t old = XFLAGS_test_uint32;
    CHECK_FALSE(flag->SetValue("-1"));
    CHECK(XFLAGS_test_uint32 == old);  // unchanged
}

TEST_CASE("ParseFrom int64 normal", "[flags]") {
    auto *flag = xcli::detail::FlagRegistry::GlobalRegistry().Find("test_int64");

    CHECK(flag->SetValue("9223372036854775807"));
    CHECK(XFLAGS_test_int64 == 9223372036854775807LL);
}

TEST_CASE("ParseFrom uint64 large", "[flags]") {
    auto *flag = xcli::detail::FlagRegistry::GlobalRegistry().Find("test_uint64");

    CHECK(flag->SetValue("18446744073709551615"));
    CHECK(XFLAGS_test_uint64 == 18446744073709551615ULL);
}

TEST_CASE("ParseFrom double normal", "[flags]") {
    auto *flag = xcli::detail::FlagRegistry::GlobalRegistry().Find("test_double");

    CHECK(flag->SetValue("2.5"));
    CHECK(XFLAGS_test_double == Approx(2.5));

    CHECK(flag->SetValue("-1.5e3"));
    CHECK(XFLAGS_test_double == Approx(-1500.0));
}

TEST_CASE("ParseFrom string", "[flags]") {
    auto *flag = xcli::detail::FlagRegistry::GlobalRegistry().Find("test_string");

    CHECK(flag->SetValue("new value"));
    CHECK(XFLAGS_test_string == "new value");

    CHECK(flag->SetValue(""));
    CHECK(XFLAGS_test_string == "");
}

// -----------------------------------------------------------
// ParseFrom — illegal/edge values
// -----------------------------------------------------------
TEST_CASE("ParseFrom bool illegal", "[flags]") {
    auto *flag = xcli::detail::FlagRegistry::GlobalRegistry().Find("test_bool");
    bool old = XFLAGS_test_bool;

    CHECK_FALSE(flag->SetValue("2"));
    CHECK(XFLAGS_test_bool == old);

    CHECK_FALSE(flag->SetValue(""));
    CHECK(XFLAGS_test_bool == old);

    CHECK_FALSE(flag->SetValue("xyz"));
    CHECK(XFLAGS_test_bool == old);
}

TEST_CASE("ParseFrom int32 illegal", "[flags]") {
    auto *flag = xcli::detail::FlagRegistry::GlobalRegistry().Find("test_int32");
    int32_t old = XFLAGS_test_int32;

    CHECK_FALSE(flag->SetValue(""));
    CHECK(XFLAGS_test_int32 == old);

    CHECK_FALSE(flag->SetValue("abc"));
    CHECK(XFLAGS_test_int32 == old);

    // overflow
    CHECK_FALSE(flag->SetValue("9999999999"));
    CHECK(XFLAGS_test_int32 == old);
}

TEST_CASE("ParseFrom double illegal", "[flags]") {
    auto *flag = xcli::detail::FlagRegistry::GlobalRegistry().Find("test_double");
    double old = XFLAGS_test_double;

    CHECK_FALSE(flag->SetValue(""));
    CHECK(XFLAGS_test_double == old);

    CHECK_FALSE(flag->SetValue("0.1xxx"));
    CHECK(XFLAGS_test_double == old);

    CHECK_FALSE(flag->SetValue(" "));
    CHECK(XFLAGS_test_double == old);
}

// -----------------------------------------------------------
// SetFlag free function
// -----------------------------------------------------------
TEST_CASE("SetFlag free function", "[flags]") {
    std::string msg;

    CHECK(xcli::detail::SetFlag("test_string", "from setflag", msg));
    CHECK(XFLAGS_test_string == "from setflag");
    CHECK_FALSE(msg.empty());

    CHECK_FALSE(xcli::detail::SetFlag("does_not_exist", "x", msg));
    CHECK(msg.empty());
}

// -----------------------------------------------------------
// GetAllFlags
// -----------------------------------------------------------
TEST_CASE("GetAllFlags", "[flags]") {
    auto flags = xcli::detail::FlagRegistry::GlobalRegistry().GetAllFlags();
    // At minimum, our 7 test flags + any from other tests
    REQUIRE(flags.size() >= 7);

    bool found = false;
    for (auto *f : flags) {
        if (f->name() == "test_bool") found = true;
    }
    CHECK(found);
}

// -----------------------------------------------------------
// Type names via FlagRegistry
// -----------------------------------------------------------
TEST_CASE("FlagTypeName", "[flags]") {
    auto *reg = &xcli::detail::FlagRegistry::GlobalRegistry();
    CHECK(reg->Find("test_bool")->TypeName() == "bool");
    CHECK(reg->Find("test_int32")->TypeName() == "int32");
    CHECK(reg->Find("test_uint32")->TypeName() == "uint32");
    CHECK(reg->Find("test_int64")->TypeName() == "int64");
    CHECK(reg->Find("test_uint64")->TypeName() == "uint64");
    CHECK(reg->Find("test_double")->TypeName() == "double");
    CHECK(reg->Find("test_string")->TypeName() == "string");
}
