#include "ActiveSurfaceMap.h"

const TilesMapping::TilesRepresentationMap TilesMapping::TileRepresentationMapping::tilesRepresentationMap = TilesMapping::TileRepresentationMapping::createMap();

TilesMapping::ActiveSurfaceMap::Tile* TilesMapping::ActiveSurfaceMap::getTileByPosition(std::pair<unsigned int, unsigned int> coordinates)
{
	for (auto& tile : tiles)
	{
		if (tile.coordinates == coordinates)
		{
			return &tile;
		}
	}

	IOUtil::Asserts::assertMessageFormatted(false, "TilesMapping::ActiveSurfaceMap::getTileByPosition - Tile not found at [%d - %d]", coordinates.first, coordinates.second);
	return nullptr;
}

TilesMapping::ActiveSurfaceMap::ActiveSurfaceMap(const std::pair<unsigned int, unsigned int> tilesSize)
	:
	xSize(tilesSize.first),
	ySize(tilesSize.second)
{
	//! Y loop
	for (unsigned int i = 0; i < tilesSize.second; i++)
	{
		//! X loop
		for (unsigned int j = 0; j < tilesSize.first; j++)
		{
			tiles.emplace_back(std::pair<unsigned int, unsigned int>{ j, i });
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

	const unsigned int tilesInitialOffset = ((electricPoleInfluenceTiles - electricPoleSideSize) / 2);
	unsigned int posX = tilesInitialOffset;
	unsigned int posY = tilesInitialOffset;

	const unsigned int distanceBetweenPoles = CalculationsUtility::Solver::calculateMaxDistanceBetweenPoles(electricPoleType);

	auto setElectrifiedArea = [&](std::pair<unsigned int, unsigned int> startPos)
		{
			for (int i = 0; i < electricPoleInfluenceTiles; i++)
			{
				for (int j = 0; j < electricPoleInfluenceTiles; j++)
				{
					Tile* tile = getTileByPosition(startPos);
					if (tile != nullptr)
					{
						tile->isElectrified = true;
					}
					startPos.first++;
				}
				startPos.first -= electricPoleInfluenceTiles;
				startPos.second++;
			}
		};

	for (auto& electricPole : electricPoles)
	{
		Tile* tile = getTileByPosition({ posX,posY });
		if (tile != nullptr)
		{
			setElectrifiedArea({ posX - tilesInitialOffset,posY - tilesInitialOffset });
		}

		posX += distanceBetweenPoles;
		if (posX > xSize)
		{
			posX = tilesInitialOffset;
			posY += distanceBetweenPoles;
			if (posY > ySize) { break; }
		}
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
			if (tile.isElectrified) { std::cout << TileRepresentationMapping::tilesRepresentationMap.at(TilesMapping::TileRepresentation::EMPTY_ELECTRIFIED_SPACE); }
			else { std::cout << TileRepresentationMapping::tilesRepresentationMap.at(TilesMapping::TileRepresentation::EMPTY_SPACE); }
		}
		else
		{
			switch (tile.entity->getEntityType())
			{
			case Entities::ENTITY_TYPE::ELECTRIC_POLE:
				std::cout << TileRepresentationMapping::tilesRepresentationMap.at(TilesMapping::TileRepresentation::ELECTRIC_POLE);
				break;
			default:
				break;
			}
		}

		idx++;
		idx = (idx == xSize) ? 0 : idx;
		if (idx == 0) { std::cout << '\n'; }
	}
}
