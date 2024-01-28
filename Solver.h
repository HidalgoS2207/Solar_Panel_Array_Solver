#pragma once

#include "Entity.h"

#include <iostream>
#include <vector>

class Solver
{
public:
	template<typename T>
	static void instantiateEntities(unsigned int numEntities, std::vector<T>& entities)
	{
		for (int i = 0; i < numEntities; i++)
		{
			entities.emplace_back();
		}
	}

	template<typename T>
	static unsigned int calculateOccupiedSurface(std::vector<T>& entities)
	{
		unsigned int ret = 0;
		for (T& entity : entities)
		{
			const std::pair<unsigned int, unsigned int>& tilesDist = entity.getTilesDistribution();

			ret += (tilesDist.first * tilesDist.second);
		}

		return ret;
	}

	static void calculateNumberOfPolesByEfectiveArea(const unsigned int efectiveArea, const Entities::ELECTRIC_POLE_TYPE preferredElectricPoleType, unsigned int& numPoles);
};