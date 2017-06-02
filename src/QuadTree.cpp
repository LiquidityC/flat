#include "QuadTree.h"
#include "RenderData.h"
#include "Camera.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <vector>

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

		SDL_Rect box = e->getEntityProperties().getBoundingBox();
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
		// TODO(Linus): This isn't safe. If >10 entities appear right in the center of the root node
		// we will have an infinite loop.
		int dw = bounds.getWidth()/2;
		int dh = bounds.getHeight()/2;

		nodes.push_back(new QuadTree(Square(
					bounds.getXpos(),
					bounds.getYpos(),
					dw,
					dh), depth + 1));
		nodes.push_back(new QuadTree(Square(
					bounds.getXpos() + dw,
					bounds.getYpos(),
					dw,
					dh), depth + 1));
		nodes.push_back(new QuadTree(Square(
					bounds.getXpos(),
					bounds.getYpos() + dh,
					dw,
					dh), depth + 1));
		nodes.push_back(new QuadTree(Square(
					bounds.getXpos() + dw,
					bounds.getYpos() + dh,
					dw,
					dh), depth + 1));

		for (auto it = objects.begin(); it != objects.end(); ++it) {
			Position pos = getNodePositionFor(*it);
			insertInto(*it, pos);
		}
		objects.clear();
	}

	void QuadTree::insertInto(Entity* e, Position p)
	{
		// TODO(Linus): Do something about this switch. It occurs in more places
		switch (p) {
			case CENTER:
				nodes[0]->insert(e);
				nodes[1]->insert(e);
				nodes[2]->insert(e);
				nodes[3]->insert(e);
				break;
			case LEFT:
				nodes[0]->insert(e);
				nodes[2]->insert(e);
				break;
			case RIGHT:
				nodes[1]->insert(e);
				nodes[3]->insert(e);
				break;
			case TOP:
				nodes[0]->insert(e);
				nodes[1]->insert(e);
				break;
			case BOTTOM:
				nodes[2]->insert(e);
				nodes[3]->insert(e);
				break;
			case TOP_LEFT:
				nodes[0]->insert(e);
				break;
			case TOP_RIGHT:
				nodes[1]->insert(e);
				break;
			case BOTTOM_LEFT:
				nodes[2]->insert(e);
				break;
			case BOTTOM_RIGHT:
				nodes[3]->insert(e);
				break;
			default:
				break;
		}
	}

	void QuadTree::insert(Entity *e)
	{
		if (nodes.empty()) {
			objects.push_back(e);
			if (objects.size() > MAX_OBJECTS) {
				split();
			}
		} else {
			Position pos = getNodePositionFor(e);
			insertInto(e, pos);
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

	void QuadTree::retrieve(std::vector<Entity*> *returnEntities, const Entity *entity) const
	{
		if (!nodes.empty()) {
			Position p = getNodePositionFor(entity);
			// TODO(Linus): Do something about this switch. It occurs in more places
			// Idea: Create a function with a callback that does the switch. The callback takes
			// QuadTree pointers as an argument. That way you can specify what method you want
			// executed on the child nodes specified by the position.
			switch (p) {
				case CENTER:
					nodes[0]->retrieve(returnEntities, entity);
					nodes[1]->retrieve(returnEntities, entity);
					nodes[2]->retrieve(returnEntities, entity);
					nodes[3]->retrieve(returnEntities, entity);
					break;
				case LEFT:
					nodes[0]->retrieve(returnEntities, entity);
					nodes[2]->retrieve(returnEntities, entity);
					break;
				case RIGHT:
					nodes[1]->retrieve(returnEntities, entity);
					nodes[3]->retrieve(returnEntities, entity);
					break;
				case TOP:
					nodes[0]->retrieve(returnEntities, entity);
					nodes[1]->retrieve(returnEntities, entity);
					break;
				case BOTTOM:
					nodes[2]->retrieve(returnEntities, entity);
					nodes[3]->retrieve(returnEntities, entity);
					break;
				case TOP_LEFT:
					nodes[0]->retrieve(returnEntities, entity);
					break;
				case TOP_RIGHT:
					nodes[1]->retrieve(returnEntities, entity);
					break;
				case BOTTOM_LEFT:
					nodes[2]->retrieve(returnEntities, entity);
					break;
				case BOTTOM_RIGHT:
					nodes[3]->retrieve(returnEntities, entity);
					break;
				default:
					break;
			}
		}

		returnEntities->insert(returnEntities->end(), objects.begin(), objects.end());
	}

	void QuadTree::render(const RenderData *renderData) const
	{
		SDL_Renderer *renderer = renderData->getRenderer();
		Camera *camera = renderData->getCamera();
		int xpos = camera->getScreenXposFor(bounds.getXpos());
		int ypos = camera->getScreenYposFor(bounds.getYpos());

		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xff);
		SDL_Rect rect = { xpos, ypos, bounds.getWidth(), bounds.getHeight() };
		SDL_RenderDrawRect(renderer, &rect);

		for (auto it = nodes.begin(); it != nodes.end(); ++it) {
			(*it)->render(renderData);
		}
	}

} // namespace flat2d
