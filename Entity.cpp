#include "Entity.h"

const std::map<Entities::ELECTRIC_POLE_TYPE, unsigned int> Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence = Entities::ElectricPoleInfluenceTilesByType::createMap();
const std::map<Entities::ELECTRIC_POLE_TYPE, unsigned int> Entities::ElectricPoleAreaOccupiedByType::ElectricPoleAreaOccupied = Entities::ElectricPoleAreaOccupiedByType::createMap();

Entities::Entity::Entity(const std::vector<bool> tilesMap, std::pair<unsigned int, unsigned int > tilesDistribution, std::pair<unsigned int, unsigned int > position)
	:
	tiles(0),
	tilesDistribution(tilesDistribution)
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

void Entities::Entity::setPosition(std::pair<unsigned int, unsigned int> newPosition)
{
	position = newPosition;
}

Entities::SolarPanel::SolarPanel()
	:
	Entity({ true,true,true,true,true,true }, { 3,3 }, { 0,0 })
{}

Entities::SolarPanel::~SolarPanel()
{}

Entities::ElectricPole::ElectricPole(const std::vector<bool> tilesMap, std::pair<unsigned int, unsigned int > tilesDistribution, unsigned int influenceTiles, double wireTilesDistance, ELECTRIC_POLE_TYPE electricPoleType)
	:
	Entity(tilesMap, tilesDistribution, { 0,0 }),
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

Entities::SmallElectricPole::SmallElectricPole()
	:
	ElectricPole({ true }, { 1,1 }, ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(ELECTRIC_POLE_TYPE::SMALL), 7.5, ELECTRIC_POLE_TYPE::SMALL)
{}

Entities::SmallElectricPole::~SmallElectricPole()
{}

Entities::MediumElectricPole::MediumElectricPole()
	:
	ElectricPole({ true }, { 1,1 }, ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(ELECTRIC_POLE_TYPE::MEDIUM), 9.0, ELECTRIC_POLE_TYPE::MEDIUM)
{}

Entities::MediumElectricPole::~MediumElectricPole()
{}

Entities::BigElectricPole::BigElectricPole()
	:
	ElectricPole({ true,true,true,true }, { 2,2 }, ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(ELECTRIC_POLE_TYPE::BIG), 30.0, ELECTRIC_POLE_TYPE::BIG)
{}

Entities::BigElectricPole::~BigElectricPole()
{}

Entities::SubStation::SubStation()
	:
	ElectricPole({ true,true,true,true }, { 2,2 }, ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(ELECTRIC_POLE_TYPE::SUBSTATION), 18.0, ELECTRIC_POLE_TYPE::SUBSTATION)
{}

Entities::SubStation::~SubStation()
{}

Entities::Accumulator::Accumulator()
	:
	Entity({ true,true,true,true }, { 2,2 }, { 0,0 })
{}

Entities::Accumulator::~Accumulator()
{}


