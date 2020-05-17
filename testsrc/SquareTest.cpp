#include "../src/Square.h"
#include "catch.hpp"
#include <iostream>

using namespace flat2d;

TEST_CASE("Test point matching", "[square]")
{
	Square lop(100, 100, 100);

	REQUIRE(lop.containsPoint(150, 150));
	REQUIRE(lop.containsPoint(105, 105));
	REQUIRE(lop.containsPoint(100, 100));
	REQUIRE(lop.containsPoint(200, 200));

	REQUIRE(!lop.containsPoint(201, 201));
	REQUIRE(!lop.containsPoint(50, 50));
	REQUIRE(!lop.containsPoint(99, 99));
}

TEST_CASE("Test equality", "[square]")
{
	Square lop1(10, 20, 40);
	Square lop2(10, 20, 40);
	Square lop3(10, 10, 40);

	REQUIRE(lop1 == lop2);
	REQUIRE(!(lop1 == lop3));
	REQUIRE(lop1 != lop3);
	REQUIRE(!(lop1 != lop2));
}

TEST_CASE("Test less operator", "[square]")
{
	Square lop1(10, 20, 40);
	Square lop2(10, 20, 40);
	Square lop3(10, 10, 40);
	Square lop4(5, 30, 40);

	REQUIRE(!(lop1 < lop2));
	REQUIRE(!(lop2 < lop1));
	REQUIRE(lop3 < lop1);
	REQUIRE(lop4 < lop1);
	REQUIRE(!(lop1 < lop3));
	REQUIRE(!(lop1 < lop4));
}
