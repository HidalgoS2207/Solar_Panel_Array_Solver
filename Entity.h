#pragma once

#include <map>
#include <vector>

namespace Entities
{
	using uintPair = std::pair<unsigned int, unsigned int>;

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

		const uintPair& getTilesDistribution() const;
		const uintPair& getPosition() const;
		ENTITY_TYPE getEntityType() const;
		const bool getIsPlaced() const;
		const char* getEntityName() const;
		const uint32_t getEntityId() const;

		void setPosition(uintPair newPosition);

		inline static void resetEntities(std::vector<Entity*>& entities)
		{
			for (Entity* entity : entities)
			{
				entity->resetEntityPosition();
			}
		}

		inline static void resetEntity(Entity* entity)
		{
			entity->resetEntityPosition();
		}

		template<typename T>
		inline static void insertToEntityPtrList(std::vector<T*>& inList, std::vector<Entity*>& outList)
		{
			for (T* inElem : inList)
			{
				outList.push_back(dynamic_cast<Entity*>(inElem));
			}
		}
	protected:
		Entity(const std::vector<bool> tilesMap, const std::pair<unsigned int, unsigned int > tilesDistribution, std::pair<unsigned int, unsigned int > position, const ENTITY_TYPE entityType, const char* entityName, const uint32_t entityId);

		const uint32_t entityId;

		unsigned int tiles;
		std::vector<bool> tilesDistMap;
		uintPair tilesDistribution;
		uintPair position;
		
		const char* entityName;
		const ENTITY_TYPE entityType;

		bool isPlaced;

	private:
		inline void resetEntityPosition()
		{
			this->isPlaced = false;
			this->position = { 0,0 };
		}
	};

	class SolarPanel : public Entity
	{
	public:
		SolarPanel(const uint32_t entityId);
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
		const std::vector<Entities::ElectricPole*> getNeighbours();
		ELECTRIC_POLE_TYPE getElectricPoleType() const;

		void setNeighbour(ElectricPole* electricPole);
		void removeNeighbour(ElectricPole* electricPole);
	protected:
		ElectricPole(const std::vector<bool> tilesMap, std::pair<unsigned int, unsigned int > tilesDistribution, unsigned int influenceTiles, double wireTilesDistance, ELECTRIC_POLE_TYPE electricPoleType, const char* entityName, const uint32_t entityId);
	private:
		const unsigned int influenceTiles;
		const double wireTilesDistance;
		const ELECTRIC_POLE_TYPE electricPoleType;
		std::vector<ElectricPole*> neighbours;
	};

	class SmallElectricPole : public ElectricPole
	{
	public:
		SmallElectricPole(const uint32_t entityId);
		~SmallElectricPole();
	private:
	};

	class MediumElectricPole : public ElectricPole
	{
	public:
		MediumElectricPole(const uint32_t entityId);
		~MediumElectricPole();
	private:
	};

	class BigElectricPole : public ElectricPole
	{
	public:
		BigElectricPole(const uint32_t entityId);
		~BigElectricPole();
	private:
	};

	class SubStation : public ElectricPole
	{
	public:
		SubStation(const uint32_t entityId);
		~SubStation();
	private:
	};

	class Accumulator : public Entity
	{
	public:
		Accumulator(const uint32_t entityId);
		~Accumulator();
	private:
	};

	class Roboport : public Entity
	{
	public:
		Roboport(const uint32_t entityId);
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
		Radar(const uint32_t entityId);
		~Radar();

		uintPair getNearbyPulseScanArea() const;
	private:
		uintPair nearbyPulseScanArea;
	};
}

