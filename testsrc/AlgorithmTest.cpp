#include "catch.hpp"
#include "../src/Algorithm.h"

TEST_CASE( "AlgorithmTest", "[algorithm]" )
{
	SECTION( "CantorPairingFunction", "[pairing]" )
	{
		REQUIRE( flat2d::Algorithm::cantorPairing(1, 2) == 8 );
		REQUIRE( flat2d::Algorithm::cantorPairing(2, 1) == 8 );
		REQUIRE( flat2d::Algorithm::cantorPairing(2, 2) == 12 );
	}
}
