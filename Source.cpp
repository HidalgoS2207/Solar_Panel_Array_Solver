#pragma once

#include <cmath>
#include <iostream>
#include <random>
#include <chrono>

#include "Entity.h"
#include "Util.h"
#include "Json.h"
#include "gfx.h"

int main(int argc, char* argv[])
{
	using EntityPtrList = std::vector<Entities::Entity*>;
	EntityPtrList entityList;

	EntitiesUtility::EntitiesIdGenerator entitiesIdGenerator;

	CalculationsUtility::SolverSettings solverSettings;

	//! Solar Panels
	solverSettings.numSolarPanels = IOUtil::KbdIO::userInputPrompt("\nNumber of solar panels required:\n> ", "Error. Out of range.\n", { 1,999 }, 0);
	//! Electric Poles preference
	solverSettings.electricPoleType = static_cast<Entities::ELECTRIC_POLE_TYPE>(
		IOUtil::KbdIO::userInputPrompt(
			"Electric Pole type preference:",
			{ "Small","Medium","Big","Substation" },
			"Error. Out of range.\n",
			{ static_cast<int>(Entities::ELECTRIC_POLE_TYPE::SMALL),static_cast<int>(Entities::ELECTRIC_POLE_TYPE::SUBSTATION) }
		)
		);
	solverSettings.numPoles = IOUtil::KbdIO::userInputPrompt("\nNumber of wanted Electric Poles:\n> ", "Error. Out of range.\n", { 1,9999 }, 0);
	//! Preferences Settings
	solverSettings.polesArrangementMethod = static_cast<CalculationsUtility::PolesArrangementMethod>(
		IOUtil::KbdIO::userInputPrompt(
			"Poles Arrangement preffered method",
			{ "Linear","Rectangular" },
			"Error. Out of range.\n",
			{ static_cast<int>(CalculationsUtility::PolesArrangementMethod::LINEAR),static_cast<int>(CalculationsUtility::PolesArrangementMethod::RECTANGULAR) }
		)
		);
	//! Accumulators
	solverSettings.numAccumulators = static_cast<unsigned int>(std::ceil((static_cast<double>(solverSettings.numSolarPanels) * CalculationsUtility::AccumulatorToSolarPanelsRatio)));

	std::cout << "\nNumber of Solar Panels = " << solverSettings.numSolarPanels << "\n";
	std::cout << "Number of Accumulators = " << solverSettings.numAccumulators << "\n";

	std::vector<Entities::SolarPanel*> solarPanels;
	CalculationsUtility::Solver::instantiateEntities(solverSettings.numSolarPanels, solarPanels, entitiesIdGenerator);
	const int totalOccupiedSurfaceSolarPanels = CalculationsUtility::Solver::calculateOccupiedSurface(solarPanels);
	std::vector<Entities::Accumulator*> accumulators;
	CalculationsUtility::Solver::instantiateEntities(solverSettings.numAccumulators, accumulators, entitiesIdGenerator);
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

	std::vector<Entities::ElectricPole*> electricPoles;
	CalculationsUtility::Solver::instantiateEntities(solverSettings.numPoles, electricPoles, solverSettings.electricPoleType, entitiesIdGenerator);

	solverSettings.entitiesArrangementStrategy = static_cast<CalculationsUtility::EntityArrangementStrategy>(
		IOUtil::KbdIO::userInputPrompt(
			"Entities Arrangement Strategy:",
			{ "Radial In First","Radial Out First","Linear Horizontal","Linear Vertical","Random","Alternate Vertical","Alternate Horizontal" },
			"Error. Out of range.\n",
			{ static_cast<int>(CalculationsUtility::EntityArrangementStrategy::RADIAL_IN_FIRST),static_cast<int>(CalculationsUtility::EntityArrangementStrategy::ALTERNATE_HOR) }
		)
		);
	solverSettings.entitiesSpawnStrategy = static_cast<CalculationsUtility::EntitySpawnStrategy>(
		IOUtil::KbdIO::userInputPrompt(
			"Entites Spawm Strategy:",
			{ "Full Random","Weighted Random","Full Sequential","Weighted Sequential" },
			"Error. Out of range.\n",
			{ static_cast<int>(CalculationsUtility::EntitySpawnStrategy::FULL_RANDOM),static_cast<int>(CalculationsUtility::EntitySpawnStrategy::WEIGHTED_SEQUENCIAL) }
		)
		);

	Entities::Entity::insertToEntityPtrList(solarPanels, entityList);
	Entities::Entity::insertToEntityPtrList(accumulators, entityList);
	Entities::Entity::insertToEntityPtrList(electricPoles, entityList);

	// ! Rendering
	GFX::Window renderHandler;
	renderHandler.declareRendereable(0, Entities::ENTITY_TYPE::SOLAR_PANEL);
	renderHandler.declareRendereable(1, Entities::ENTITY_TYPE::SOLAR_PANEL);
	renderHandler.declareRendereable(2, Entities::ENTITY_TYPE::SOLAR_PANEL);

	std::chrono::steady_clock::time_point t1;
	std::chrono::nanoseconds t_diff;
	constexpr double BASE_FPS = 60.0;
	constexpr double FPS = 120.0;
	constexpr double NANOS_PER_FRAME = (1000.0) / (FPS) * (1000000.0);
	double timeCount = 0.0;
	while (renderHandler.windowState() == true)
	{
		t_diff = std::chrono::steady_clock::now() - t1;

		if (t_diff.count() >= NANOS_PER_FRAME)
		{
			t1 = std::chrono::steady_clock::now();

			//for (Entities::Entity* entityPtr : entityList)
			//{
			//	renderHandler.updateRendereablePosition(entityPtr->getEntityId(), entityPtr->getPosition());
			//}
			renderHandler.updateRendereablePosition(0, { 50,50 });
			renderHandler.updateRendereablePosition(1, { 100,50 });
			renderHandler.updateRendereablePosition(2, { 200,50 });

			renderHandler.render();

			renderHandler.handleEvents();

			timeCount += (t_diff.count() / 1000000000.0);
		}
	}

	CalculationsUtility::Solver::calculateArrangement(solverSettings, solarPanels, accumulators, electricPoles);

	Output::Json json;
	json.saveToFile("bluePrintJsonOutput.txt", entityList);

	CalculationsUtility::Solver::destroyEntities(solarPanels);
	CalculationsUtility::Solver::destroyEntities(accumulators);
	CalculationsUtility::Solver::destroyEntities(electricPoles);

	return 0;
}

//! @TODO
// Find a list of tileable distributions based on input
// Eventually the user input could be electric pole type wanted and plant max power output