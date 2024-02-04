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
	solverSettings.numSolarPanels = IOUtil::KbdIO::userInputPrompt("\nNumber of solar panels required:\n> ", "Error. Out of range.\n", { 1,999 }, 0);
	//! Electric Poles preference
	solverSettings.electricPoleType = static_cast<Entities::ELECTRIC_POLE_TYPE>(
		IOUtil::KbdIO::userInputPrompt(
			"\nElectrical Pole type Preference:\n1.Small\n2.Medium\n3.Big\n4.Substation\n> ", 
			"Error. Out of range.\n",
			{ static_cast<int>(Entities::ELECTRIC_POLE_TYPE::SMALL),static_cast<int>(Entities::ELECTRIC_POLE_TYPE::SUBSTATION) },
			0)
		);
	solverSettings.numPoles = IOUtil::KbdIO::userInputPrompt("\nNumber of wanted Electric Poles:\n> ", "Error. Out of range.\n", { 1,9999 }, 0);
	//! Preferences Settings
	solverSettings.polesArrangementMethod = static_cast<CalculationsUtility::PolesArrangementMethod>(
		IOUtil::KbdIO::userInputPrompt(
			"\nPoles Arrangemnet Preffered Method:\n1.Linear\n2.Rectangular\n> ",
			"Error. Out of range.\n",
			{ static_cast<int>(CalculationsUtility::PolesArrangementMethod::LINEAR),static_cast<int>(CalculationsUtility::PolesArrangementMethod::RECTANGULAR) },
			-1)
		);
	//! Accumulators
	solverSettings.numAccumulators = static_cast<unsigned int>(std::ceil((static_cast<double>(solverSettings.numSolarPanels) * CalculationsUtility::AccumulatorToSolarPanelsRatio)));

	std::cout << "\nNumber of Solar Panels = " << solverSettings.numSolarPanels << "\n";
	std::cout << "Number of Accumulators = " << solverSettings.numAccumulators << "\n";

	std::vector<Entities::SolarPanel> solarPanels;
	CalculationsUtility::Solver::instantiateEntities(solverSettings.numSolarPanels, solarPanels);
	const int totalOccupiedSurfaceSolarPanels = CalculationsUtility::Solver::calculateOccupiedSurface(solarPanels);
	std::vector<Entities::Accumulator> accumulators;
	CalculationsUtility::Solver::instantiateEntities(solverSettings.numAccumulators, accumulators);
	const int totalOccupiedSurfaceAccumulators = CalculationsUtility::Solver::calculateOccupiedSurface(accumulators);

	//! Electrical active entities
	const unsigned int totalOccupiedSurfaceActiveEntities = totalOccupiedSurfaceSolarPanels + totalOccupiedSurfaceAccumulators;
	unsigned int polesIteration = 0;
	bool activeSurfaceIsValid = false;
	while (!activeSurfaceIsValid)
	{
		activeSurfaceIsValid = true;
		unsigned int effectiveArea = 0;
		solverSettings.numPoles = CalculationsUtility::Solver::calculatePotentialMaxEffectiveArea(solverSettings, effectiveArea);
		if (effectiveArea < totalOccupiedSurfaceActiveEntities)
		{
			IOUtil::Asserts::assertMessageFormatted(false, "No available active surface to hold the components. Adding more poles!. Num Poles = %d. Iteration = %d", solverSettings.numPoles, polesIteration);
			activeSurfaceIsValid = false;
			polesIteration++;

			solverSettings.numPoles++;
		}
	}

	solverSettings.entitiesArrangementStrategy = static_cast<CalculationsUtility::EntityArrangementStrategy>(
		IOUtil::KbdIO::userInputPrompt(
			"\nEntities Arrangement Strategy:\n1.Radial In First\n2.Radial Out First\n3.Linear Horizontal\n4.Linear Vertical\n5.Random\n6.Alternate Vertical\n7.Alternate Horizontal\n> ",
			"Error. Out of range.\n",
			{ static_cast<int>(CalculationsUtility::EntityArrangementStrategy::RADIAL_IN_FIRST),static_cast<int>(CalculationsUtility::EntityArrangementStrategy::ALTERNATE_HOR) },
			-1)
		);
	solverSettings.entitiesSpawnStrategy = static_cast<CalculationsUtility::EntitySpawnStrategy>(
		IOUtil::KbdIO::userInputPrompt(
			"\nEntities Spawn Strategy:\n1.Full Random\n2.Weighted Random\n3.Full Sequential\n4.Weighted Sequential\n> ",
			"Error. Out of range.\n",
			{ static_cast<int>(CalculationsUtility::EntitySpawnStrategy::FULL_RANDOM),static_cast<int>(CalculationsUtility::EntitySpawnStrategy::WEIGHTED_SEQUENCIAL) },
			-1)
		);

	CalculationsUtility::Solver::calculateArrangement(solverSettings);

	return 0;
}

//! @TODO
// Find a list of tileable distributions based on input
// Eventually the user input could be electric pole type wanted and plant max power output