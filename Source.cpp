#pragma once

#include <cmath>
#include <iostream>

#include "Entity.h"
#include "Solver.h"

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
	Solver::instantiateEntities(numSolarPanels, solarPanels);
	const int totalOccupiedSurfaceSolarPanels = Solver::calculateOccupiedSurface(solarPanels);
	std::vector<Entities::Accumulator> accumulators;
	Solver::instantiateEntities(numAccumulators, accumulators);
	const int totalOccupiedSurfaceAccumulators = Solver::calculateOccupiedSurface(accumulators);

	//! Electrical active entities
	const int totalOccupiedSurfaceActiveEntities = totalOccupiedSurfaceSolarPanels + totalOccupiedSurfaceAccumulators;

	unsigned int numPolesRequired = 0;
	Solver::calculateNumberOfPolesByEffectiveArea(totalOccupiedSurfaceActiveEntities, preferredElectricPoleType,numPolesRequired);

	std::cout << "Number of Poles Calculated = " << numPolesRequired;

	return 0;
}

//! @TODO
// Find a list of tileable distributions based on input
// 