#include "ActiveSurfaceMap.h"

const TilesMapping::TilesRepresentationMap TilesMapping::TileRepresentationMapping::tilesRepresentationMap = TilesMapping::TileRepresentationMapping::createMap();

TilesMapping::ActiveSurfaceMap::ActiveSurfaceMap(const std::pair<unsigned int, unsigned int> tilesSize)
	:
	xSize(tilesSize.first),
	ySize(tilesSize.second)
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

TilesMapping::ActiveSurfaceMap::~ActiveSurfaceMap()
{}

void TilesMapping::ActiveSurfaceMap::insertEntity(const Entities::Entity* entity, const std::pair<unsigned int, unsigned int> coor)
{
	
}

void TilesMapping::ActiveSurfaceMap::printSurface()
{
	std::cout << "\n\n Current Surface: \n\n";

	unsigned int idx = 0;

	for (auto& tile : tiles)
	{
		if (tile.entity == nullptr)
		{
			if(tile.isElectrified) { std::cout << TileRepresentationMapping::tilesRepresentationMap.at(TilesMapping::TileRepresentation::EMPTY_ELECTRIFIED_SPACE); }
			else { std::cout << TileRepresentationMapping::tilesRepresentationMap.at(TilesMapping::TileRepresentation::EMPTY_SPACE); }
		}

		idx++;
		idx = (idx == xSize) ? 0 : idx;
		if (idx == 0) { std::cout << '\n'; }
	}
}
