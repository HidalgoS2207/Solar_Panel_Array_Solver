#pragma once

#include "Entity.h"
#include "Util.h"

#include <iostream>
#include <string>
#include <vector>
#include <math.h>

namespace TilesMapping
{
	enum class TileRepresentation
	{
		SOLAR_PANEL,
		ACCUMULATOR,
		ELECTRIC_POLE,
		EMPTY_ELECTRIFIED_SPACE,
		EMPTY_SPACE
	};
	using TilesRepresentationMap = std::map<TileRepresentation, std::string>;

	struct TileRepresentationMapping
	{
	private:
		static TilesRepresentationMap createMap()
		{
			TilesRepresentationMap auxMap;

			auxMap[TileRepresentation::SOLAR_PANEL] = 'S';
			auxMap[TileRepresentation::ACCUMULATOR] = 'A';
			auxMap[TileRepresentation::ELECTRIC_POLE] = 'P';
			auxMap[TileRepresentation::EMPTY_ELECTRIFIED_SPACE] = 'X';
			auxMap[TileRepresentation::EMPTY_SPACE] = 'O';

			return auxMap;
		}
	public:
		static const TilesRepresentationMap tilesRepresentationMap;
	};

	class ActiveSurfaceMap
	{
	private:
		using uintPairCoordinates = std::pair<unsigned int, unsigned int>;

		struct Tile
		{
			Tile(const uintPairCoordinates coordinates)
				:
				coordinates(coordinates),
				entity(nullptr),
				isElectrified(false)
			{}

			const uintPairCoordinates coordinates;
			bool isElectrified;
			Entities::Entity* entity;
		};
		using TilesByCoordinate = std::map<uintPairCoordinates, Tile*>;

		Tile* getTileByPosition(const uintPairCoordinates coordinates);
	public:
		ActiveSurfaceMap(const uintPairCoordinates tilesSize);
		~ActiveSurfaceMap();

		bool insertEntity(Entities::Entity* entity, const uintPairCoordinates coor);
		bool insertElectricPoles(std::vector<Entities::ElectricPole*>& electricPoles);
		bool getIsAvailable(const uintPairCoordinates coor) const;
		void printSurface();

		inline uintPairCoordinates getTilesMapSize() const
		{
			return uintPairCoordinates({ xSize,ySize });
		}

		inline void refreshTilesSate()
		{
			for (Tile* tile : tiles)
			{
				if (tile->entity == nullptr) { continue; }
				if (tile->entity->getIsPlaced()) { continue; }

				tile->entity = nullptr;
			}
		}
	private:
		const bool checkTilesAvailability(const uintPairCoordinates range, const uintPairCoordinates pos);

		std::vector<Tile*> tiles;
		TilesByCoordinate tilesByCoordinate;

		const unsigned int xSize;
		const unsigned int ySize;

		bool electricPolesPlaced;
	};
}


