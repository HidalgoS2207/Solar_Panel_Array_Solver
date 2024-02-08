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
		struct Tile
		{
			Tile(const std::pair<unsigned int, unsigned int> coordinates)
				:
				coordinates(coordinates),
				entity(nullptr),
				isElectrified(false)
			{}

			const std::pair<unsigned int, unsigned int> coordinates;
			bool isElectrified;
			Entities::Entity* entity;
		};

		Tile* getTileByPosition(std::pair<unsigned int, unsigned int>coordinates);
	public:
		ActiveSurfaceMap(const std::pair<unsigned int, unsigned int> tilesSize);
		~ActiveSurfaceMap();

		bool insertEntity(const Entities::Entity* entity, const std::pair<unsigned int, unsigned int> coor);
		bool insertElectricPoles(std::vector<Entities::ElectricPole*>& electricPoles);
		void printSurface();
	private:
		std::vector<Tile> tiles;

		const unsigned int xSize;
		const unsigned int ySize;

		bool electricPolesPlaced;
	};
}


