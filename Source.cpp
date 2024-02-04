#pragma once

#include <cmath>
#include <iostream>
#include <random>

#include "Entity.h"
#include "Util.h"

int main(int argc, char* argv[])
{
	CalculationsUtility::SolverSettings solverSettings;

	//! Solar Panels
	solverSettings.numSolarPanels = IOUtil::KbdIO::userInputPrompt("\nNumber of solar panels required:\n>", "Error. Out of range.\n", { 1,999 }, 0);
	//! Electric Poles preference
	solverSettings.electricPoleType = static_cast<Entities::ELECTRIC_POLE_TYPE>(
		IOUtil::KbdIO::userInputPrompt("\nElectrical Pole type Preference:\n1.Small\n2.Medium\n3.Big\n4.Substation\n>", "Error. Out of range.\n", { static_cast<int>(Entities::ELECTRIC_POLE_TYPE::SMALL), static_cast<int>(Entities::ELECTRIC_POLE_TYPE::SUBSTATION) }, 0)
		);
	solverSettings.numPoles = IOUtil::KbdIO::userInputPrompt("\nNumber of wanted Electric Poles:\n>", "Error. Out of range.\n", { 1,9999 }, 0);
	//! Preferences Settings
	solverSettings.polesArrangementMethod = static_cast<CalculationsUtility::PolesArrangementMethod>(
		IOUtil::KbdIO::userInputPrompt("\nPoles Arrangemnet Preffered Method:\n1.Linear\n2.Rectangular\n>", "Error. Out of range.\n", { 1,2 }, -1)
		);
	//! Accumulators
	solverSettings.numAccumulators = static_cast<unsigned int>(std::ceil((static_cast<double>(solverSettings.numSolarPanels) * CalculationsUtility::AccumulatorToSolarPanelsRatio)));
	
	std::cout << "Number of Solar Panels = " << solverSettings.numSolarPanels << "\n";
	std::cout << "Number of Accumulators = " << solverSettings.numAccumulators << "\n";

	std::vector<Entities::SolarPanel> solarPanels;
	CalculationsUtility::Solver::instantiateEntities(solverSettings.numSolarPanels, solarPanels);
	const int totalOccupiedSurfaceSolarPanels = CalculationsUtility::Solver::calculateOccupiedSurface(solarPanels);
	std::vector<Entities::Accumulator> accumulators;
	CalculationsUtility::Solver::instantiateEntities(solverSettings.numAccumulators, accumulators);
	const int totalOccupiedSurfaceAccumulators = CalculationsUtility::Solver::calculateOccupiedSurface(accumulators);
	std::vector<Entities::ElectricPole*> electricPoles;
	CalculationsUtility::Solver::instantiateEntities(solverSettings.numAccumulators, accumulators);
	const int totalOccupiedSurfaceAccumulators = CalculationsUtility::Solver::calculateOccupiedSurface(accumulators);

	//! Electrical active entities
	const unsigned int totalOccupiedSurfaceActiveEntities = totalOccupiedSurfaceSolarPanels + totalOccupiedSurfaceAccumulators;
	
	unsigned int effectiveArea = 0;
	CalculationsUtility::Solver::calculatePotentialMaxEffectiveArea(solverSettings,effectiveArea);
	IOUtil::Asserts::assertMessage((effectiveArea >= totalOccupiedSurfaceActiveEntities), "Not available active surface to hold the components");

	solverSettings.entitiesArrangementStrategy = CalculationsUtility::EntityArrangementStrategy::RANDOM;
	solverSettings.entitiesSpawnStrategy = CalculationsUtility::EntitySpawnStrategy::FULL_RANDOM;

	return 0;
}

//! @TODO
// Find a list of tileable distributions based on input
// 