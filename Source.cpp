#pragma once

#include <cmath>
#include <iostream>
#include <random>

#include "Entity.h"
#include "Util.h"

int main(int argc, char* argv[])
{
	//! Solar Panels
	const unsigned int numSolarPanels = IOUtil::KbdIO::userInputPrompt("\nNumber of solar panels required:\n>", "Error. Out of range.\n", { 1,999 }, 0);
	//! Electric Poles preference
	const Entities::ELECTRIC_POLE_TYPE prefferedElectricPoleType = static_cast<Entities::ELECTRIC_POLE_TYPE>(
		IOUtil::KbdIO::userInputPrompt("\nElectrical Pole type Preference:\n1.Small\n2.Medium\n3.Big\n4.Substation\n>", "Error. Out of range.\n", { static_cast<int>(Entities::ELECTRIC_POLE_TYPE::SMALL), static_cast<int>(Entities::ELECTRIC_POLE_TYPE::SUBSTATION) }, 0)
		);
	const unsigned int numPolesRequired = IOUtil::KbdIO::userInputPrompt("\nNumber of wanted Electric Poles:\n>", "Error. Out of range.\n", { 1,9999 }, 0);
	//! Preferences Settings
	const bool calculateForSelfContained = static_cast<bool>(
		IOUtil::KbdIO::userInputPrompt("\nCalculate for self contained system [Full tilable]:\n1.Yes\n2.No\n>", "Error. Out of range.\n", { 1,2 }, -1)
		);
	const CalculationsUtility::PolesArrangementMethod polesArrangementMethod = static_cast<CalculationsUtility::PolesArrangementMethod>(
		IOUtil::KbdIO::userInputPrompt("\nPoles Arrangemnet Preffered Method:\n1.Linear\n2.Rectangular\n>", "Error. Out of range.\n", { 1,2 }, -1)
		);
	//! Accumulators
	const unsigned int numAccumulators = std::ceil((static_cast<double>(numSolarPanels) * CalculationsUtility::AccumulatorToSolarPanelsRatio));
	
	std::cout << "Number of Solar Panels = " << numSolarPanels << "\n";
	std::cout << "Number of Accumulators = " << numAccumulators << "\n";

	std::vector<Entities::SolarPanel> solarPanels;
	CalculationsUtility::Solver::instantiateEntities(numSolarPanels, solarPanels);
	const int totalOccupiedSurfaceSolarPanels = CalculationsUtility::Solver::calculateOccupiedSurface(solarPanels);
	std::vector<Entities::Accumulator> accumulators;
	CalculationsUtility::Solver::instantiateEntities(numAccumulators, accumulators);
	const int totalOccupiedSurfaceAccumulators = CalculationsUtility::Solver::calculateOccupiedSurface(accumulators);

	//! Electrical active entities
	const int totalOccupiedSurfaceActiveEntities = totalOccupiedSurfaceSolarPanels + totalOccupiedSurfaceAccumulators;
	
	unsigned int effectiveArea = 0;
	CalculationsUtility::Solver::calculatePotentialMaxEffectiveArea(prefferedElectricPoleType, polesArrangementMethod,numPolesRequired,effectiveArea);

	std::cout << "Efective Area : " << effectiveArea << "\n";

	return 0;
}

//! @TODO
// Find a list of tileable distributions based on input
// 