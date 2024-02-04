#pragma once

#include "Entity.h"

#include <vector>
#include <string>

class ActiveSurfaceMap
{
private:
	struct Tile
	{
		Tile(const std::pair<unsigned int, unsigned int> coordinates)
			:
			coordinates(coordinates),
			entity(nullptr)
		{}

		const std::pair<unsigned int, unsigned int> coordinates;
		Entities::Entity* entity;
	};
public:
	ActiveSurfaceMap(const std::pair<unsigned int,unsigned int> tilesSize);
	~ActiveSurfaceMap();

	void insertEntity(const Entities::Entity* entity, const std::pair<unsigned int, unsigned int> coor);
private:
	std::vector<Tile> tiles;
};