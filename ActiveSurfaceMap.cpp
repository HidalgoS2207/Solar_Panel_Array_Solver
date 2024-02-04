#include "ActiveSurfaceMap.h"

ActiveSurfaceMap::ActiveSurfaceMap(const std::pair<unsigned int, unsigned int> tilesSize)
{
	//! Y loop
	for (unsigned int i = 0; i < tilesSize.first; i++)
	{
		//! X loop
		for (unsigned int j = 0; j < tilesSize.second; j++)
		{
			tiles.emplace_back(std::pair<unsigned int,unsigned int>{ j,i });
		}
	}
}

ActiveSurfaceMap::~ActiveSurfaceMap()
{}

void ActiveSurfaceMap::insertEntity(const Entities::Entity* entity, const std::pair<unsigned int, unsigned int> coor)
{
	
}
