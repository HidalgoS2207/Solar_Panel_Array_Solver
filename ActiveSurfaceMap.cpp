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

void TilesMapping::ActiveSurfaceMap::insertElectricPoles(const std::vector<Entities::ElectricPole*>& electricPoles)
{
	//! Rigth now all electric poles are uniform and the system doesn't manage combination of different ones
	//! It is safe to extract the type from the first element of the array
	IOUtil::Asserts::assertMessage(electricPoles.size(), "TilesMapping::ActiveSurfaceMap::insertElectricPoles - Electric Poles Array is empty!");
	Entities::ELECTRIC_POLE_TYPE electricPoleType = electricPoles.front()->getElectricPoleType();

	const unsigned int electricPoleOccupiedArea = Entities::ElectricPoleAreaOccupiedByType::ElectricPoleAreaOccupied.at(electricPoleType);
	const unsigned int electricPoleSideSize = static_cast<unsigned int>(std::sqrt(electricPoleOccupiedArea));
	const unsigned int electricPoleInfluenceTiles = Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(electricPoleType);

	unsigned int posX = 0;
	unsigned int posY = 0;

	const unsigned int tilesStartOffset = ((electricPoleInfluenceTiles - electricPoleSideSize) / 2);

	for (auto& electricPole : electricPoles)
	{

	}
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
		else
		{

		}

		idx++;
		idx = (idx == xSize) ? 0 : idx;
		if (idx == 0) { std::cout << '\n'; }
	}
}
