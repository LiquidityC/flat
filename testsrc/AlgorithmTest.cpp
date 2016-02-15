#include "catch.hpp"
#include "../src/Algorithm.h"

TEST_CASE( "AlgorithmTest", "[algorithm]" )
{
	SECTION( "CantorPairingFunction", "[pairing]" )
	{
		REQUIRE( Algorithm::cantorPairing(1, 2) == 8 );
		REQUIRE( Algorithm::cantorPairing(2, 1) == 8 );
		REQUIRE( Algorithm::cantorPairing(2, 2) == 12 );
	}
}
