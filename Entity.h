#pragma once

#include <map>
#include <vector>

namespace Entities
{
	using uintPair = std::pair<unsigned int,unsigned int>;

	const unsigned int SolarPanelSideNumTiles = 3;
	const unsigned int AccumulatorSideNumTiles = 2;
	const unsigned int RoboportSideNumTiles = 4;
	const unsigned int RadarSideNumTiles = 2;

	//This constant should allow to separate influence areas without the risk of leaving a one tile gap with no entities
	const unsigned int maxGapBetweenPolesInfluenceArea = (SolarPanelSideNumTiles);

	enum class ENTITY_TYPE
	{
		SOLAR_PANEL,
		ACCUMULATOR,
		ELECTRIC_POLE,
		ROBOPORT,
		RADAR
	};

	enum ELECTRIC_POLE_TYPE
	{
		INVALID = -1,
		SMALL = 0,
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
		ENTITY_TYPE getEntityType() const;
		const char* getEntityTypeStr() const;
		const bool getIsPlaced() const;

		void setPosition(std::pair<unsigned int, unsigned int> newPosition);

		inline static void resetEntities(std::vector<Entity*>& entities)
		{
			for (Entity* entity : entities)
			{
				entity->resetEntityPosition();
			}
		}
	protected:
		Entity(const std::vector<bool> tilesMap, const std::pair<unsigned int, unsigned int > tilesDistribution, std::pair<unsigned int, unsigned int > position, const ENTITY_TYPE entityType);

		unsigned int tiles;
		std::vector<bool> tilesDistMap;
		std::pair<unsigned int, unsigned int> tilesDistribution;
		std::pair<unsigned int, unsigned int> position;

		const ENTITY_TYPE entityType;

		bool isPlaced;

	private:
		inline void resetEntityPosition()
		{
			this->isPlaced = false;
			this->position = { 0,0 };
		}

		static inline const char* localizeEntityType(ENTITY_TYPE entityType)
		{
			switch (entityType)
			{
			case Entities::ENTITY_TYPE::SOLAR_PANEL:
				return "Solar Panel";
				break;
			case Entities::ENTITY_TYPE::ACCUMULATOR:
				return "Accumulator";
				break;
			case Entities::ENTITY_TYPE::ELECTRIC_POLE:
				return "Electric Pole";
				break;
			case Entities::ENTITY_TYPE::ROBOPORT:
				return "Roboport";
				break;
			case Entities::ENTITY_TYPE::RADAR:
				return "Radar";
				break;
			default:
				//IOUtil::Asserts::assertMessageFormatted(false, "Cannot localize entity type of type %d", entityType);
				return "";
				break;
			}
		}
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

		unsigned int getInfluenceTiles() const;
		double getWireTilesDistance() const;
		int getInfluenceArea() const;
		ELECTRIC_POLE_TYPE getElectricPoleType() const;

		void setNeighbour(ElectricPole* electricPole);
		void removeNeighbour(ElectricPole* electricPole);
	protected:
		ElectricPole(const std::vector<bool> tilesMap, std::pair<unsigned int, unsigned int > tilesDistribution, unsigned int influenceTiles, double wireTilesDistance, ELECTRIC_POLE_TYPE electricPoleType);
	private:
		const unsigned int influenceTiles;
		const double wireTilesDistance;
		const ELECTRIC_POLE_TYPE electricPoleType;
		std::vector<ElectricPole*> neighbours;
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

	class Roboport : public Entity
	{
	public:
		Roboport();
		~Roboport();

		uintPair getConstructionArea() const;
		uintPair getSupplyArea() const;
	private:
		uintPair constructionArea;
		uintPair supplyArea;
	};

	class Radar : public Entity
	{
	public:
		Radar();
		~Radar();

		uintPair getNearbyPulseScanArea() const;
	private:
		uintPair nearbyPulseScanArea;
	};
}

