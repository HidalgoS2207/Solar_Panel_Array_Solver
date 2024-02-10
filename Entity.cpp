#include "Entity.h"

const std::map<Entities::ELECTRIC_POLE_TYPE, unsigned int> Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence = Entities::ElectricPoleInfluenceTilesByType::createMap();
const std::map<Entities::ELECTRIC_POLE_TYPE, unsigned int> Entities::ElectricPoleAreaOccupiedByType::ElectricPoleAreaOccupied = Entities::ElectricPoleAreaOccupiedByType::createMap();
const std::map<Entities::ELECTRIC_POLE_TYPE, double> Entities::ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance = Entities::ElectricPoleWireTilesDistanceByType::createMap();

Entities::Entity::Entity(const std::vector<bool> tilesMap, const std::pair<unsigned int, unsigned int > tilesDistribution, std::pair<unsigned int, unsigned int > position, const ENTITY_TYPE entityType)
	:
	tiles(0),
	tilesDistribution(tilesDistribution),
	position(position),
	entityType(entityType),
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

const std::pair<unsigned int, unsigned int>& Entities::Entity::getTilesDistribution() const
{
	return tilesDistribution;
}

const std::pair<unsigned int, unsigned int>& Entities::Entity::getPosition() const
{
	return position;
}

Entities::ENTITY_TYPE Entities::Entity::getEntityType() const
{
	return entityType;
}

const char* Entities::Entity::getEntityTypeStr() const
{
	return Entities::Entity::localizeEntityType(entityType);
}

const bool Entities::Entity::getIsPlaced() const
{
	return isPlaced;
}

void Entities::Entity::setPosition(std::pair<unsigned int, unsigned int> newPosition)
{
	position = newPosition;
	isPlaced = true;
}

Entities::SolarPanel::SolarPanel()
	:
	Entity({ true,true,true,true,true,true }, { SolarPanelSideNumTiles,SolarPanelSideNumTiles }, { 0,0 }, ENTITY_TYPE::SOLAR_PANEL)
{}

Entities::SolarPanel::~SolarPanel()
{}

Entities::ElectricPole::ElectricPole(const std::vector<bool> tilesMap, std::pair<unsigned int, unsigned int > tilesDistribution, unsigned int influenceTiles, double wireTilesDistance, ELECTRIC_POLE_TYPE electricPoleType)
	:
	Entity(tilesMap, tilesDistribution, { 0,0 }, ENTITY_TYPE::ELECTRIC_POLE),
	influenceTiles(influenceTiles),
	wireTilesDistance(wireTilesDistance),
	electricPoleType(electricPoleType)
{
}

Entities::ElectricPole::~ElectricPole()
{}

unsigned int Entities::ElectricPole::getInfluenceTiles()
{
	return influenceTiles;
}

double Entities::ElectricPole::getWireTilesDistance()
{
	return wireTilesDistance;
}

int Entities::ElectricPole::getInfluenceArea()
{
	return (influenceTiles * influenceTiles) - (tilesDistribution.first * tilesDistribution.second);
}

Entities::ELECTRIC_POLE_TYPE Entities::ElectricPole::getElectricPoleType()
{
	return electricPoleType;
}

Entities::SmallElectricPole::SmallElectricPole()
	:
	ElectricPole
	(
		{ true },
		{ 1,1 },
		ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(ELECTRIC_POLE_TYPE::SMALL),
		ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(ELECTRIC_POLE_TYPE::SMALL),
		ELECTRIC_POLE_TYPE::SMALL)
{}

Entities::SmallElectricPole::~SmallElectricPole()
{}

Entities::MediumElectricPole::MediumElectricPole()
	:
	ElectricPole
	(
		{ true },
		{ 1,1 },
		ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(ELECTRIC_POLE_TYPE::MEDIUM),
		ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(ELECTRIC_POLE_TYPE::MEDIUM),
		ELECTRIC_POLE_TYPE::MEDIUM
	)
{}

Entities::MediumElectricPole::~MediumElectricPole()
{}

Entities::BigElectricPole::BigElectricPole()
	:
	ElectricPole
	(
		{ true,true,true,true },
		{ 2,2 },
		ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(ELECTRIC_POLE_TYPE::BIG),
		ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(ELECTRIC_POLE_TYPE::BIG),
		ELECTRIC_POLE_TYPE::BIG
	)
{}

Entities::BigElectricPole::~BigElectricPole()
{}

Entities::SubStation::SubStation()
	:
	ElectricPole
	(
		{ true,true,true,true },
		{ 2,2 },
		ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(ELECTRIC_POLE_TYPE::SUBSTATION),
		ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(ELECTRIC_POLE_TYPE::SUBSTATION),
		ELECTRIC_POLE_TYPE::SUBSTATION
	)
{}

Entities::SubStation::~SubStation()
{}

Entities::Accumulator::Accumulator()
	:
	Entity({ true,true,true,true }, { AccumulatorSideNumTiles,AccumulatorSideNumTiles }, { 0,0 }, ENTITY_TYPE::ACCUMULATOR)
{}

Entities::Accumulator::~Accumulator()
{}


