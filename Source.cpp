#pragma once

#include <cmath>
#include <iostream>

#include "Entity.h"

template<typename T>
void instantiateEntities(unsigned int numEntities, std::vector<T>& entities)
{
	for (int i = 0; i < numEntities; i++)
	{
		entities.emplace_back();
	}
}

template<typename T>
unsigned int calculateOccupiedSurface(std::vector<T>& entities)
{
	unsigned int ret = 0;
	for (T& entity : entities)
	{
		const std::pair<unsigned int, unsigned int>& tilesDist = entity.getTilesDistribution();

		ret += (tilesDist.first * tilesDist.second);
	}

	return ret;
}

int main(int argc, char* argv[])
{
	const double AccumulatorToSolarPanelsRatio = 0.84;

	int solarPanelsInput = 0;
	int electricPoleTypePreference = static_cast<int>(Entities::ELECTRIC_POLE_TYPE::INVALID);

	auto verifyInputRange = [](int min, int max, int val) -> bool
		{
			return (val <= max && val >= min);
		};

	auto userInputPrompt = [&](const char* promptText, const char* errorText, const std::pair<int, int> inputRange, int& inputVar)
		{
			while (!verifyInputRange(inputRange.first, inputRange.second, inputVar))
			{
				std::cout << promptText;
				std::cin >> inputVar;
				if (!verifyInputRange(inputRange.first, inputRange.second, inputVar))
				{
					std::cout << errorText;
				}
			}
		};

	userInputPrompt("Number of solar panels required:\n>", "Error. Out of range.\n", { 1,999 }, solarPanelsInput);
	userInputPrompt("Electrical Pole type Preference:\n1.Small\n2.Medium\n3.Big\n4.Substation\n", "Error. Out of range.\n", { static_cast<int>(Entities::ELECTRIC_POLE_TYPE::SMALL), static_cast<int>(Entities::ELECTRIC_POLE_TYPE::SUBSTATION) }, electricPoleTypePreference);

	//! Solar Panels
	const unsigned int numSolarPanels = solarPanelsInput;
	//! Accumulators
	const unsigned int numAccumulators = std::ceil((static_cast<double>(numSolarPanels) * AccumulatorToSolarPanelsRatio));
	//! Electric Poles preference
	const Entities::ELECTRIC_POLE_TYPE preferredElectricPoleType = static_cast<Entities::ELECTRIC_POLE_TYPE>(electricPoleTypePreference);

	std::cout << "Number of solar Panels = " << numSolarPanels << "\n";
	std::cout << "Number of Accumulators = " << numAccumulators << "\n";

	std::vector<Entities::SolarPanel> solarPanels;
	instantiateEntities(numSolarPanels, solarPanels);
	const int totalOccupiedSurfaceSolarPanels = calculateOccupiedSurface(solarPanels);
	std::vector<Entities::Accumulator> accumulators;
	instantiateEntities(numAccumulators, accumulators);
	const int totalOccupiedSurfaceAccumulators = calculateOccupiedSurface(accumulators);

	//! Electrical active entities
	const int totalOccupiedSurfaceActiveEntities = totalOccupiedSurfaceSolarPanels + totalOccupiedSurfaceAccumulators;

	return 0;
}

//! @TODO
// Find a list of tileable distributions based on input
// 