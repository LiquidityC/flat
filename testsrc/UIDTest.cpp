#include <iostream>
#include "catch.hpp"
#include "../src/UID.h"

using namespace flat2d;

TEST_CASE( "UID Generation sequence test", "[UID]")
{
	// Generation doesn't start at 10 since other objects have been created
	// during the test execution
	size_t index = UID::generate();

	for (size_t i = index + 1; i < index + 11; i++) {
		REQUIRE( i == UID::generate());
	}
}
