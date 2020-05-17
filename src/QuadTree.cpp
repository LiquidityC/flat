#include "QuadTree.h"
#include <SDL.h>
#include <algorithm>
#include <iostream>
#include <vector>

namespace flat2d {
	int QuadTree::getNodeIndexFor(const Entity* e) const
	{
		int dw = bounds.getXpos() + (bounds.getWidth() / 2);
		int dh = bounds.getYpos() + (bounds.getHeight() / 2);
		bool left = false;
		bool right = false;

		SDL_Rect box = e->getEntityProperties().getBoundingBox();
		if (box.x + box.w < dw) {
			left = true;
		} else if (box.x > dw) {
			right = true;
		}

		if (left) {
			if (box.y + box.h < dh) {
				return 0;
			} else if (box.y > dh) {
				return 2;
			}
		} else if (right) {
			if (box.y + box.h < dh) {
				return 1;
			} else if (box.y > dh) {
				return 3;
			}
		}

		return -1;
	}

	void QuadTree::split()
	{
		int dw = bounds.getWidth() / 2;
		int dh = bounds.getHeight() / 2;

		nodes.push_back(new QuadTree(
		  Square(bounds.getXpos(), bounds.getYpos(), dw, dh), depth + 1));
		nodes.push_back(new QuadTree(
		  Square(bounds.getXpos() + dw, bounds.getYpos(), dw, dh), depth + 1));
		nodes.push_back(new QuadTree(
		  Square(bounds.getXpos(), bounds.getYpos() + dh, dw, dh), depth + 1));
		nodes.push_back(new QuadTree(
		  Square(bounds.getXpos() + dw, bounds.getYpos() + dh, dw, dh),
		  depth + 1));

		std::vector<Entity*> tempObjects;
		for (auto it = objects.begin(); it != objects.end(); ++it) {
			int index = getNodeIndexFor(*it);
			if (index == -1) {
				tempObjects.push_back(*it);
				continue;
			}
			nodes[index]->insert(*it);
		}
		objects.clear();
		for (auto it = tempObjects.begin(); it != tempObjects.end(); ++it) {
			objects.push_back(*it);
		}
	}

	void QuadTree::insert(Entity* e)
	{
		if (nodes.empty()) {
			objects.push_back(e);
			if (objects.size() > MAX_OBJECTS) {
				split();
			}
		} else {
			int index = getNodeIndexFor(e);
			if (index == -1) {
				objects.push_back(e);
			} else {
				nodes[index]->insert(e);
			}
		}
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

	void QuadTree::retrieve(std::vector<Entity*>* returnEntities,
	                        const Entity* entity) const
	{
		int index = getNodeIndexFor(entity);
		if (index != -1 && !nodes.empty()) {
			nodes[index]->retrieve(returnEntities, entity);
		}

		returnEntities->insert(
		  returnEntities->end(), objects.begin(), objects.end());
	}

} // namespace flat2d
