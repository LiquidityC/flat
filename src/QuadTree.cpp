#include "CollisionDetector.h"
#include "QuadTree.h"
#include "RenderData.h"
#include "Camera.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

namespace flat2d
{
	QuadTree::Position QuadTree::getNodePositionFor(const Entity *e) const
	{
		// TODO(Linus): This isn't good. We need to handle the "Entity outside root area" case.
		// Perhaps just exclude these entities from the calculations.
		int dw = bounds.getXpos() + (bounds.getWidth()/2);
		int dh = bounds.getYpos() + (bounds.getHeight()/2);
		bool left = false;
		bool right = false;
		bool top = false;
		bool bottom = false;


		EntityShape box;
		if (deltatimeMonitor != nullptr) {
			box = e->getEntityProperties().getVelocityColliderShape(deltatimeMonitor->getDeltaTime());
		} else {
			box = e->getEntityProperties().getColliderShape();
		}

		if (box.x + box.w < dw) {
			left = true;
		} else if (box.x > dw) {
			right = true;
		}

		if (box.y + box.h < dh) {
			top = true;
		} else if (box.y > dh) {
			bottom = true;
		}

		if (left) {
			if (top) {
				return TOP_LEFT;
			} else if (bottom) {
				return BOTTOM_LEFT;
			}
			return LEFT;
		} else if (right) {
			if (top) {
				return TOP_RIGHT;
			} else if (bottom) {
				return BOTTOM_RIGHT;
			}
			return RIGHT;
		}

		if (top) {
			return TOP;
		} else if (bottom) {
			return BOTTOM;
		}

		return CENTER;
	}

	void QuadTree::split()
	{
		int x = bounds.getXpos();
		int y = bounds.getYpos();
		int dw = bounds.getWidth()/2;
		int dh = bounds.getHeight()/2;

		nodes.push_back(new QuadTree(Square(x, y, dw, dh), deltatimeMonitor, depth + 1));
		nodes.push_back(new QuadTree(Square(x + dw, y, dw, dh), deltatimeMonitor, depth + 1));
		nodes.push_back(new QuadTree(Square(x, y + dh, dw, dh), deltatimeMonitor, depth + 1));
		nodes.push_back(new QuadTree(Square(x + dw, y + dh, dw, dh), deltatimeMonitor, depth + 1));

		for (auto it = objects.begin(); it != objects.end(); ++it) {
			Position pos = getNodePositionFor(*it);
			insertInto(*it, pos);
		}
		objects.clear();
	}

	void QuadTree::unsplit()
	{
		assert (objects.size() == 0);
		assert (nodes.size() == 4);

		nodes[0]->gather(&objects);
		nodes[1]->gather(&objects);
		nodes[2]->gather(&objects);
		nodes[3]->gather(&objects);

		nodes[0]->clear();
		nodes[1]->clear();
		nodes[2]->clear();
		nodes[3]->clear();

		nodes.clear();

		if (splitAvailable()) {
			split();
		}
	}

	void QuadTree::gather(std::vector<Entity*> *collection) {
		if (!nodes.empty()) {
			nodes[0]->gather(collection);
			nodes[1]->gather(collection);
			nodes[2]->gather(collection);
			nodes[3]->gather(collection);
		} else {
			collection->insert(collection->end(), objects.begin(), objects.end());
		}
	}

	unsigned int QuadTree::count() const
	{
		if (!nodes.empty()) {
			unsigned int count = 0;
			count += nodes[0]->count();
			count += nodes[1]->count();
			count += nodes[2]->count();
			count += nodes[3]->count();
			return count;
		} else {
			return objects.size();
		}
	}

	void QuadTree::insertInto(Entity* e, Position p)
	{
		distribute(p, [e] (QuadTree *node)
				   {
						node->insert(e);
				   });
	}

	void QuadTree::distribute(const Position& p, DistributionCallback cb)
	{
		switch (p) {
			case CENTER:
				cb(nodes[0]);
				cb(nodes[1]);
				cb(nodes[2]);
				cb(nodes[3]);
				break;
			case LEFT:
				cb(nodes[0]);
				cb(nodes[2]);
				break;
			case RIGHT:
				cb(nodes[1]);
				cb(nodes[3]);
				break;
			case TOP:
				cb(nodes[0]);
				cb(nodes[1]);
				break;
			case BOTTOM:
				cb(nodes[2]);
				cb(nodes[3]);
				break;
			case TOP_LEFT:
				cb(nodes[0]);
				break;
			case TOP_RIGHT:
				cb(nodes[1]);
				break;
			case BOTTOM_LEFT:
				cb(nodes[2]);
				break;
			case BOTTOM_RIGHT:
				cb(nodes[3]);
				break;
			default:
				break;
		}
	}

	void QuadTree::insert(Entity *e)
	{
		if (splitAvailable()) {
			split();
		}

		if (!nodes.empty()) {
			Position pos = getNodePositionFor(e);
			insertInto(e, pos);
		} else {
			objects.push_back(e);
		}
	}

	bool QuadTree::splitAvailable() const
	{
		bool result = true;
		result = nodes.empty() && result;
		result = objects.size() > MAX_OBJECTS && result;
		result = depth < 100 && result;
		result = (bounds.getWidth() > 10 && bounds.getHeight() > 10) && result;
		return result;
	}

	void QuadTree::clear()
	{
		objects.clear();
		if (!nodes.empty()) {
			nodes[0]->clear();
			nodes[1]->clear();
			nodes[2]->clear();
			nodes[3]->clear();

			delete nodes[0];
			delete nodes[1];
			delete nodes[2];
			delete nodes[3];
		}

		nodes.clear();
	}

	int QuadTree::getDepth() const
	{
		if (nodes.empty()) {
			return depth;
		}

		int d = depth;
		d = std::max(nodes[0]->getDepth(), d);
		d = std::max(nodes[1]->getDepth(), d);
		d = std::max(nodes[2]->getDepth(), d);
		d = std::max(nodes[3]->getDepth(), d);

		return d;
	}

	void QuadTree::retrieve(std::vector<Entity*> *returnEntities, const Entity *entity)
	{
		if (!nodes.empty()) {
			Position p = getNodePositionFor(entity);
			distribute(p, [returnEntities, entity] (QuadTree *node) -> void
					   {
					   		node->retrieve(returnEntities, entity);
					   });
		} else {
			returnEntities->insert(returnEntities->end(), objects.begin(), objects.end());
		}
	}

	void QuadTree::render(const RenderData *renderData) const
	{
		SDL_Renderer *renderer = renderData->getRenderer();
		Camera *camera = renderData->getCamera();
		int xpos = camera->getScreenXposFor(bounds.getXpos());
		int ypos = camera->getScreenYposFor(bounds.getYpos());

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0xff);
		SDL_Rect rect = { xpos, ypos, bounds.getWidth(), bounds.getHeight() };
		SDL_RenderDrawRect(renderer, &rect);

		for (auto it = nodes.begin(); it != nodes.end(); ++it) {
			(*it)->render(renderData);
		}
	}

	void QuadTree::purge(std::vector<Entity*> *purgedEntities)
	{
		if (!nodes.empty()) {
			unsigned int total = purgedEntities->size();
			nodes[0]->purge(purgedEntities);
			nodes[1]->purge(purgedEntities);
			nodes[2]->purge(purgedEntities);
			nodes[3]->purge(purgedEntities);

			if (purgedEntities->size() != total && count() <= MAX_OBJECTS) {
				unsplit();
			}

		} else {
			auto it = objects.begin();
			while (it != objects.end()) {
				if ((*it)->getEntityProperties().hasLocationChanged()) {
					purgedEntities->push_back(*it);
					it = objects.erase(it);
				} else {
					++it;
				}
			}
		}
	}

} // namespace flat2d
