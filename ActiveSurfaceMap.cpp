#include "ActiveSurfaceMap.h"

const TilesMapping::TilesRepresentationMap TilesMapping::TileRepresentationMapping::tilesRepresentationMap = TilesMapping::TileRepresentationMapping::createMap();

TilesMapping::ActiveSurfaceMap::Tile* TilesMapping::ActiveSurfaceMap::getTileByPosition(const uintPairCoordinates coordinates)
{
	if (tilesByCoordinate.find(coordinates) == tilesByCoordinate.end()) 
	{ 
		IOUtil::Asserts::assertMessageFormatted(!verboseExecution, "TilesMapping::ActiveSurfaceMap::getTileByPosition - Tile not found at [%d - %d]", coordinates.first, coordinates.second);
		return nullptr; 
	}
	return tilesByCoordinate[coordinates];
}

TilesMapping::ActiveSurfaceMap::ActiveSurfaceMap(const uintPairCoordinates tilesSize)
	:
	xSize(tilesSize.first),
	ySize(tilesSize.second),
	electricPolesPlaced(false),
	/*debug vars*/
	verboseExecution(false)
{
	//! Y loop
	for (unsigned int i = 0; i < tilesSize.second; i++)
	{
		//! X loop
		for (unsigned int j = 0; j < tilesSize.first; j++)
		{
			Tile* tilePtr = new Tile(uintPairCoordinates{ j, i });
			tiles.push_back(tilePtr);
			tilesByCoordinate[{j, i}] = tilePtr;
		}
	}
}

TilesMapping::ActiveSurfaceMap::~ActiveSurfaceMap()
{
	for (Tile* tile : tiles)
	{
		delete tile;
	}
}

bool TilesMapping::ActiveSurfaceMap::insertEntity(Entities::Entity* entity, const uintPairCoordinates coor)
{
	const unsigned int xSizeEntity = entity->getTilesDistribution().first;
	const unsigned int ySizeEntity = entity->getTilesDistribution().second;

	if (checkTilesAvailability(entity->getTilesDistribution(), coor))
	{
		for (int i = coor.second; i < entity->getTilesDistribution().second + coor.second; i++)
		{
			for (int j = coor.first; j < entity->getTilesDistribution().first + coor.first; j++)
			{
				Tile* tile = this->getTileByPosition({ j,i });
				if (!entity->getIsPlaced()) { entity->setPosition({ j,i }); }
				tile->entity = entity;
			}
		}
	}

	return entity->getIsPlaced();
}

bool TilesMapping::ActiveSurfaceMap::insertElectricPoles(std::vector<Entities::ElectricPole*>& electricPoles)
{
	//! First assume success, any assert in the process will set this flag to false
	electricPolesPlaced = true;

	auto setAssertStatement = [&](const char* assertMsg) -> bool
		{
			electricPolesPlaced = false;
			IOUtil::Asserts::assertMessage(electricPolesPlaced, assertMsg);
			return electricPolesPlaced;
		};

	if (electricPoles.size() == 0)
	{
		return setAssertStatement("TilesMapping::ActiveSurfaceMap::insertElectricPoles - Electric Poles Array is empty!");
	}

	//! Rigth now all electric poles are uniform and the system doesn't manage combination of different ones
	//! It is safe to extract the type from the first element of the array
	Entities::ELECTRIC_POLE_TYPE electricPoleType = electricPoles.front()->getElectricPoleType();

	const unsigned int electricPoleOccupiedArea = Entities::ElectricPoleAreaOccupiedByType::ElectricPoleAreaOccupied.at(electricPoleType);
	const unsigned int electricPoleSideSize = static_cast<unsigned int>(std::sqrt(electricPoleOccupiedArea));
	const unsigned int electricPoleInfluenceTiles = Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(electricPoleType);

	const unsigned int tilesInitialOffset = ((electricPoleInfluenceTiles - electricPoleSideSize) / 2);
	unsigned int posX = tilesInitialOffset;
	unsigned int posY = tilesInitialOffset;

	const unsigned int distanceBetweenPoles = CalculationsUtility::Solver::calculateMaxDistanceBetweenPoles(electricPoleType);

	auto setElectricPole = [&](Entities::ElectricPole* electricPole, uintPairCoordinates startPos) -> bool
		{
			if (!electricPole->getIsPlaced())
			{
				for (int i = 0; i < electricPoleSideSize; i++)
				{
					for (int j = 0; j < electricPoleSideSize; j++)
					{
						Tile* tile = getTileByPosition(startPos);
						if (tile != nullptr)
						{
							tile->entity = dynamic_cast<Entities::Entity*>(electricPole);
							if (!electricPole->getIsPlaced())
							{
								electricPole->setPosition(startPos);
							}
						}
						else
						{
							return setAssertStatement("TilesMapping::ActiveSurfaceMap::insertElectricPoles - Tile is nullptr");
						}
						startPos.first++;
					}
					startPos.first -= electricPoleSideSize;
					startPos.second++;
				}

				return electricPolesPlaced;
			}
			else
			{
				return setAssertStatement("TilesMapping::ActiveSurfaceMap::insertElectricPoles - Electric pole already placed!");
			}
		};

	auto setElectrifiedArea = [&](uintPairCoordinates startPos) -> bool
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
					else
					{
						return setAssertStatement("TilesMapping::ActiveSurfaceMap::setElectrifiedArea - Tile is nullptr!");
					}
					startPos.first++;
				}
				startPos.first -= electricPoleInfluenceTiles;
				startPos.second++;
			}

			return electricPolesPlaced;
		};

	for (auto& electricPole : electricPoles)
	{
		if (!setElectrifiedArea({ posX - tilesInitialOffset,posY - tilesInitialOffset })) { break; };
		if (!setElectricPole(electricPole, { posX,posY })) { break; }

		posX += distanceBetweenPoles;
		if (posX > xSize)
		{
			posX = tilesInitialOffset;
			posY += distanceBetweenPoles;
			if (posY > ySize) { break; }
		}
	}

	return electricPolesPlaced;
}

bool TilesMapping::ActiveSurfaceMap::getIsAvailable(const uintPairCoordinates coor) const
{
	return (tilesByCoordinate.find(coor) != tilesByCoordinate.end());
}

void TilesMapping::ActiveSurfaceMap::printSurface()
{
	std::cout << "\n\n Current Surface: \n\n";

	unsigned int idx = 0;

	for (auto& tile : tiles)
	{
		if (tile->entity == nullptr)
		{
			if (tile->isElectrified) { std::cout << TileRepresentationMapping::tilesRepresentationMap.at(TilesMapping::TileRepresentation::EMPTY_ELECTRIFIED_SPACE); }
			else { std::cout << TileRepresentationMapping::tilesRepresentationMap.at(TilesMapping::TileRepresentation::EMPTY_SPACE); }
		}
		else
		{
			switch (tile->entity->getEntityType())
			{
			case Entities::ENTITY_TYPE::ELECTRIC_POLE:
				std::cout << TileRepresentationMapping::tilesRepresentationMap.at(TilesMapping::TileRepresentation::ELECTRIC_POLE);
				break;
			case Entities::ENTITY_TYPE::SOLAR_PANEL:
				std::cout << TileRepresentationMapping::tilesRepresentationMap.at(TilesMapping::TileRepresentation::SOLAR_PANEL);
				break;
			case Entities::ENTITY_TYPE::ACCUMULATOR:
				std::cout << TileRepresentationMapping::tilesRepresentationMap.at(TilesMapping::TileRepresentation::ACCUMULATOR);
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

const bool TilesMapping::ActiveSurfaceMap::checkTilesAvailability(const uintPairCoordinates range, const uintPairCoordinates pos)
{
	const unsigned int xOffset = pos.first + range.first;
	const unsigned int yOffset = pos.second + range.second;

	unsigned int electrifiedTiles = 0;

	for (int i = pos.second; i < yOffset; i++)
	{
		for (int j = pos.first; j < xOffset; j++)
		{
			Tile* tile = this->getTileByPosition({ j,i });
			if (tile == nullptr) { return false; }
			if (tile->entity != nullptr) { return false; }
			if (tile->isElectrified) { electrifiedTiles++; }
		}
	}

	return (electrifiedTiles > 0);
}
