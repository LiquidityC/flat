#ifndef _GAME_OBJECT_IMPL_H
#define _GAME_OBJECT_IMPL_H

#include "../src/Entity.h"

class EntityImpl : public flat2d::Entity
{
  public:
	EntityImpl(unsigned int x, unsigned int y)
	  : Entity(x, y, 10, 10)
	{
		entityProperties.setCollidable(true);
		entityProperties.setColliderShape({ 0, 0, 10, 10 });
	}
};

#endif
