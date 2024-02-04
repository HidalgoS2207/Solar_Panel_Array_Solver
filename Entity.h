#pragma once

#include <vector>
#include <map>

namespace Entities
{
	const unsigned int SolarPanelSideNumTiles = 3;
	const unsigned int AccumulatorSideNumTiles = 2;

	//This constant should allow to separate influence areas without the risk of leaving a one tile gap with no entities
	const unsigned int maxGapBetweenPolesInflueceArea = (SolarPanelSideNumTiles);

	enum class ENTITY_TYPE
	{
		SOLAR_PANEL,
		ACCUMULATOR,
		ELECTRIC_POLE
	};

	enum ELECTRIC_POLE_TYPE
	{
		INVALID = 0,
		SMALL = 1,
		MEDIUM,
		BIG,
		SUBSTATION
	};

	struct ElectricPoleInfluenceTilesByType
	{
	private:
		static std::map<ELECTRIC_POLE_TYPE, unsigned int> createMap()
		{
			std::map<ELECTRIC_POLE_TYPE, unsigned int> auxMap;

			auxMap[ELECTRIC_POLE_TYPE::SMALL] = 5;
			auxMap[ELECTRIC_POLE_TYPE::MEDIUM] = 7;
			auxMap[ELECTRIC_POLE_TYPE::BIG] = 4;
			auxMap[ELECTRIC_POLE_TYPE::SUBSTATION] = 18;

			return auxMap;
		}
	public:
		static const std::map<ELECTRIC_POLE_TYPE, unsigned int> ElectricPoleInfluence;
	};

	struct ElectricPoleAreaOccupiedByType
	{
	private:
		static std::map<ELECTRIC_POLE_TYPE, unsigned int> createMap()
		{
			std::map<ELECTRIC_POLE_TYPE, unsigned int> auxMap;

			auxMap[ELECTRIC_POLE_TYPE::SMALL] = 1;
			auxMap[ELECTRIC_POLE_TYPE::MEDIUM] = 1;
			auxMap[ELECTRIC_POLE_TYPE::BIG] = 4;
			auxMap[ELECTRIC_POLE_TYPE::SUBSTATION] = 4;

			return auxMap;
		}
	public:
		static const std::map<ELECTRIC_POLE_TYPE, unsigned int> ElectricPoleAreaOccupied;
	};

	struct ElectricPoleWireTilesDistanceByType
	{
	private:
		static std::map<ELECTRIC_POLE_TYPE, double> createMap()
		{
			std::map<ELECTRIC_POLE_TYPE, double> auxMap;

			auxMap[ELECTRIC_POLE_TYPE::SMALL] = 7.5;
			auxMap[ELECTRIC_POLE_TYPE::MEDIUM] = 9;
			auxMap[ELECTRIC_POLE_TYPE::BIG] = 30;
			auxMap[ELECTRIC_POLE_TYPE::SUBSTATION] = 18;

			return auxMap;
		}
	public:
		static const std::map<ELECTRIC_POLE_TYPE, double> ElectricPoleWireTilesDistance;
	};

	class Entity
	{
	public:
		virtual ~Entity();

		const std::pair<unsigned int, unsigned int>& getTilesDistribution() const;
		const std::pair<unsigned int, unsigned int>& getPosition() const;

		void setPosition(std::pair<unsigned int, unsigned int> newPosition);
	protected:
		Entity(const std::vector<bool> tilesMap, const std::pair<unsigned int, unsigned int > tilesDistribution, std::pair<unsigned int, unsigned int > position, const ENTITY_TYPE entityType);

		unsigned int tiles;
		std::vector<bool> tilesDistMap;
		std::pair<unsigned int, unsigned int> tilesDistribution;
		std::pair<unsigned int, unsigned int> position;

		const ENTITY_TYPE entityType;
	};

	class SolarPanel : public Entity
	{
	public:
		SolarPanel();
		~SolarPanel();
	private:
	};

	class ElectricPole : public Entity
	{
	public:
		~ElectricPole();

		unsigned int getInfluenceTiles();
		double getWireTilesDistance();

		int getInfluenceArea();
	protected:
		ElectricPole(const std::vector<bool> tilesMap, std::pair<unsigned int, unsigned int > tilesDistribution, unsigned int influenceTiles, double wireTilesDistance, ELECTRIC_POLE_TYPE electricPoleType);
	private:
		const unsigned int influenceTiles;
		const double wireTilesDistance;
		const ELECTRIC_POLE_TYPE electricPoleType;
	};

	class SmallElectricPole : public ElectricPole
	{
	public:
		SmallElectricPole();
		~SmallElectricPole();
	private:
	};

	class MediumElectricPole : public ElectricPole
	{
	public:
		MediumElectricPole();
		~MediumElectricPole();
	private:
	};

	class BigElectricPole : public ElectricPole
	{
	public:
		BigElectricPole();
		~BigElectricPole();
	private:
	};

	class SubStation : public ElectricPole
	{
	public:
		SubStation();
		~SubStation();
	private:
	};

	class Accumulator : public Entity
	{
	public:
		Accumulator();
		~Accumulator();
	private:
	};
}

