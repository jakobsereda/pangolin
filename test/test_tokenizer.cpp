#include "doctest.h"

TEST_CASE("hello world test")
{
    CHECK(1 + 1 == 2);
}

TEST_CASE("basic arithmetic operations")
{
    CHECK(2 * 3 == 6);
    CHECK(5 - 2 == 3);
    CHECK(8 / 2 == 4);
    CHECK((2 + 3) * 2 == 10);
}

TEST_CASE("comparison operators")
{
    CHECK(3 < 5);
    CHECK(10 > 2);
    CHECK(7 == 7);
    CHECK_FALSE(4 == 5);
}

TEST_CASE("edge cases")
{
    CHECK(0 + 0 == 0);
    CHECK(-1 + 1 == 0);
    CHECK(-5 * -2 == 10);
    CHECK(1000000 + 1 == 1000001);
}