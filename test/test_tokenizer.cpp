#include "doctest.h"

TEST_CASE("hello, doctest") {
	CHECK(1 + 1 == 2);
	CHECK(2 + 2 == 4);
}

TEST_CASE("doctest returns") {
	CHECK(3 * 3 == 9);
}
