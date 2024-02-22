#include "Entity.h"

const std::map<Entities::ELECTRIC_POLE_TYPE, unsigned int> Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence = Entities::ElectricPoleInfluenceTilesByType::createMap();
const std::map<Entities::ELECTRIC_POLE_TYPE, unsigned int> Entities::ElectricPoleAreaOccupiedByType::ElectricPoleAreaOccupied = Entities::ElectricPoleAreaOccupiedByType::createMap();
const std::map<Entities::ELECTRIC_POLE_TYPE, double> Entities::ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance = Entities::ElectricPoleWireTilesDistanceByType::createMap();

Entities::Entity::Entity(const std::vector<bool> tilesMap, const std::pair<unsigned int, unsigned int > tilesDistribution, std::pair<unsigned int, unsigned int > position, const ENTITY_TYPE entityType, const char* entityName, const uint32_t entityId)
	:
	tiles(0),
	tilesDistribution(tilesDistribution),
	position(position),
	entityType(entityType),
	entityName(entityName),
	entityId(entityId),
	isPlaced(false)
{
	tilesDistMap = tilesMap;
	for (bool tilesDistMapElem : tilesDistMap)
	{
		if (tilesDistMapElem) { tiles++; }
	}
}

Entities::Entity::~Entity()
{}

const Entities::uintPair& Entities::Entity::getTilesDistribution() const
{
	return tilesDistribution;
}

const Entities::uintPair& Entities::Entity::getPosition() const
{
	return position;
}

Entities::ENTITY_TYPE Entities::Entity::getEntityType() const
{
	return entityType;
}

const bool Entities::Entity::getIsPlaced() const
{
	return isPlaced;
}

const char* Entities::Entity::getEntityName() const
{
	return entityName;
}

const uint32_t Entities::Entity::getEntityId() const
{
	return entityId;
}

void Entities::Entity::setPosition(uintPair newPosition)
{
	position = newPosition;
	isPlaced = true;
}

Entities::SolarPanel::SolarPanel(const uint32_t entityId)
	:
	Entity({ true,true,true,true,true,true,true,true,true }, { SolarPanelSideNumTiles,SolarPanelSideNumTiles }, { 0,0 }, ENTITY_TYPE::SOLAR_PANEL, "solar-panel", entityId)
{}

Entities::SolarPanel::~SolarPanel()
{}

Entities::ElectricPole::ElectricPole(const std::vector<bool> tilesMap, std::pair<unsigned int, unsigned int > tilesDistribution, unsigned int influenceTiles, double wireTilesDistance, ELECTRIC_POLE_TYPE electricPoleType, const char* entityName, const uint32_t entityId)
	:
	Entity(tilesMap, tilesDistribution, { 0,0 }, ENTITY_TYPE::ELECTRIC_POLE, entityName, entityId),
	influenceTiles(influenceTiles),
	wireTilesDistance(wireTilesDistance),
	electricPoleType(electricPoleType)
{
}

Entities::ElectricPole::~ElectricPole()
{}

unsigned int Entities::ElectricPole::getInfluenceTiles() const
{
	return influenceTiles;
}

double Entities::ElectricPole::getWireTilesDistance() const
{
	return wireTilesDistance;
}

int Entities::ElectricPole::getInfluenceArea() const
{
	return (influenceTiles * influenceTiles) - (tilesDistribution.first * tilesDistribution.second);
}

const std::vector<Entities::ElectricPole*> Entities::ElectricPole::getNeighbours()
{
	return neighbours;
}

Entities::ELECTRIC_POLE_TYPE Entities::ElectricPole::getElectricPoleType() const
{
	return electricPoleType;
}

void Entities::ElectricPole::setNeighbour(ElectricPole* electricPole)
{
	for (ElectricPole* neighbour : neighbours)
	{
		if (neighbour == electricPole)
		{
			return;
		}
	}
	neighbours.push_back(electricPole);
}

void Entities::ElectricPole::removeNeighbour(ElectricPole* electricPole)
{
	std::vector<ElectricPole*>::iterator neighbourIt = neighbours.begin();
	std::vector<ElectricPole*>::iterator neighbourToEraseIt;

	for (; neighbourIt != neighbours.end(); neighbourIt++)
	{
		if (*neighbourIt == electricPole)
		{
			neighbourToEraseIt = neighbourIt;
			*neighbourToEraseIt = nullptr;
			break;
		}
	}

	neighbours.erase(neighbourToEraseIt);
	neighbours.shrink_to_fit();
}

Entities::SmallElectricPole::SmallElectricPole(const uint32_t entityId)
	:
	ElectricPole
	(
		{ true },
		{ 1,1 },
		ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(ELECTRIC_POLE_TYPE::SMALL),
		ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(ELECTRIC_POLE_TYPE::SMALL),
		ELECTRIC_POLE_TYPE::SMALL,
		"small-electric-pole",
		entityId
	)
{}

Entities::SmallElectricPole::~SmallElectricPole()
{}

Entities::MediumElectricPole::MediumElectricPole(const uint32_t entityId)
	:
	ElectricPole
	(
		{ true },
		{ 1,1 },
		ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(ELECTRIC_POLE_TYPE::MEDIUM),
		ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(ELECTRIC_POLE_TYPE::MEDIUM),
		ELECTRIC_POLE_TYPE::MEDIUM,
		"medium-electric-pole",
		entityId
	)
{}

Entities::MediumElectricPole::~MediumElectricPole()
{}

Entities::BigElectricPole::BigElectricPole(const uint32_t entityId)
	:
	ElectricPole
	(
		{ true,true,true,true },
		{ 2,2 },
		ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(ELECTRIC_POLE_TYPE::BIG),
		ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(ELECTRIC_POLE_TYPE::BIG),
		ELECTRIC_POLE_TYPE::BIG,
		"big-electric-pole",
		entityId
	)
{}

Entities::BigElectricPole::~BigElectricPole()
{}

Entities::SubStation::SubStation(const uint32_t entityId)
	:
	ElectricPole
	(
		{ true,true,true,true },
		{ 2,2 },
		ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(ELECTRIC_POLE_TYPE::SUBSTATION),
		ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(ELECTRIC_POLE_TYPE::SUBSTATION),
		ELECTRIC_POLE_TYPE::SUBSTATION,
		"substation",
		entityId
	)
{}

Entities::SubStation::~SubStation()
{}

Entities::Accumulator::Accumulator(const uint32_t entityId)
	:
	Entity({ true,true,true,true }, { AccumulatorSideNumTiles,AccumulatorSideNumTiles }, { 0,0 }, ENTITY_TYPE::ACCUMULATOR, "accumulator", entityId)
{}

Entities::Accumulator::~Accumulator()
{}

Entities::Roboport::Roboport(const uint32_t entityId)
	:
	Entity({ true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true }, { RoboportSideNumTiles,RoboportSideNumTiles }, { 0,0 }, ENTITY_TYPE::ROBOPORT, "roboport", entityId),
	supplyArea({ 50,50 }),
	constructionArea({ 110,110 })
{}

Entities::Roboport::~Roboport()
{}

Entities::uintPair Entities::Roboport::getConstructionArea() const
{
	return constructionArea;
}

Entities::uintPair Entities::Roboport::getSupplyArea() const
{
	return supplyArea;
}

Entities::Radar::Radar(const uint32_t entityId)
	:
	Entity({ true,true,true,true }, { RadarSideNumTiles,RadarSideNumTiles }, { 0,0 }, ENTITY_TYPE::RADAR, "radar", entityId),
	nearbyPulseScanArea({ 224,224 })
{}

Entities::Radar::~Radar()
{}

Entities::uintPair Entities::Radar::getNearbyPulseScanArea() const
{
	return nearbyPulseScanArea;
}
