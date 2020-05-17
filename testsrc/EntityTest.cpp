#include "../src/Entity.h"
#include "../src/Texture.h"
#include "catch.hpp"

TEST_CASE("EntityTest", "[entity]")
{
	flat2d::Entity entity(0, 0, 50, 50);

	SECTION("TextureTest", "[entity,texture]")
	{
		flat2d::Texture* t = new flat2d::Texture();
		entity.setTexture(t);
		std::weak_ptr<flat2d::Texture> wp = entity.getTexture();
		REQUIRE(wp.use_count() == 1);
		auto wp2 = wp;
		auto sp = wp.lock();
		REQUIRE(sp.use_count() == 2);
		auto sp2 = wp2.lock();
		REQUIRE(sp.use_count() == 3);

		REQUIRE(!wp.expired());
		wp.reset();
		REQUIRE(wp.expired());
		REQUIRE(!wp2.expired());
		wp2.reset();
		REQUIRE(wp2.expired());
	}

	SECTION("FixedPositionTest", "[entity]")
	{
		REQUIRE(!entity.isFixedPosition());
		entity.setFixedPosition(true);
		REQUIRE(entity.isFixedPosition());
		entity.setFixedPosition(false);
		REQUIRE(!entity.isFixedPosition());
	}

	SECTION("InputHandlerTest", "[entity]")
	{
		REQUIRE(!entity.isInputHandler());
		entity.setInputHandler(true);
		REQUIRE(entity.isInputHandler());
		entity.setInputHandler(false);
		REQUIRE(!entity.isInputHandler());
	}

	SECTION("DeadTest", "[entity]")
	{
		REQUIRE(!entity.isDead());
		entity.setDead(true);
		REQUIRE(entity.isDead());
		entity.setDead(false);
		REQUIRE(!entity.isDead());
	}
}
