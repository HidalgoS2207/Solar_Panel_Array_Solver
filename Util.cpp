#include "Util.h"

unsigned int CalculationsUtility::Solver::calculatePotentialMaxEffectiveArea(const SolverSettings& solverSettings, unsigned int& effectiveArea)
{
	unsigned int modNumPoles = solverSettings.numPoles;

	const unsigned int influenceTiles = Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(solverSettings.electricPoleType);
	const double wireDistance = Entities::ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(solverSettings.electricPoleType);
	const unsigned int maxDistance = calculateMaxDistanceBetweenPoles(solverSettings.electricPoleType);

	switch (solverSettings.polesArrangementMethod)
	{
	case CalculationsUtility::PolesArrangementMethod::LINEAR:
	{
		for (unsigned int i = 0; i < solverSettings.numPoles; i++)
		{
			effectiveArea += ((maxDistance * Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(solverSettings.electricPoleType)) - Entities::ElectricPoleAreaOccupiedByType::ElectricPoleAreaOccupied.at(solverSettings.electricPoleType));
		}
	}
	break;
	case CalculationsUtility::PolesArrangementMethod::RECTANGULAR:
	{
		const unsigned int squareOfPoles = static_cast<unsigned int>(std::ceil(std::sqrt(static_cast<double>(solverSettings.numPoles))));

		effectiveArea = (std::pow(influenceTiles, 2) - Entities::ElectricPoleAreaOccupiedByType::ElectricPoleAreaOccupied.at(solverSettings.electricPoleType)) * std::pow(squareOfPoles, 2);
		effectiveArea += (((maxDistance - influenceTiles) * influenceTiles) * (squareOfPoles - 1)) * (squareOfPoles);
		const unsigned int totalSideDistance = (squareOfPoles * influenceTiles) + ((squareOfPoles - 1) * (maxDistance - influenceTiles));
		effectiveArea += (totalSideDistance * (maxDistance - influenceTiles) * (squareOfPoles - 1));

		modNumPoles = std::pow(squareOfPoles, 2);
	}
	break;
	default:
		IOUtil::Asserts::assertMessageFormatted(false, "CalculationsUtility::Solver::calculatePotentialMaxEffectiveArea - Invalid pole arrangement method = %d", solverSettings.polesArrangementMethod);
	}

	return modNumPoles;
}

std::pair<unsigned int, unsigned int> CalculationsUtility::Solver::calculateSidesSize(const SolverSettings& solverSettings)
{
	std::pair<unsigned int, unsigned int> ret{ 0,0 };

	const unsigned int maxDistance = calculateMaxDistanceBetweenPoles(solverSettings.electricPoleType);
	const unsigned int electricPoleOccupiedArea = Entities::ElectricPoleAreaOccupiedByType::ElectricPoleAreaOccupied.at(solverSettings.electricPoleType);
	const unsigned int electricPoleSideSize = static_cast<unsigned int>(std::sqrt(electricPoleOccupiedArea));
	const unsigned int gapBetweenElectrifiedAreas = calculateGapBetweenElectrifiedAreas(solverSettings.electricPoleType);

	switch (solverSettings.polesArrangementMethod)
	{
	case CalculationsUtility::PolesArrangementMethod::LINEAR:
		ret.first = (solverSettings.numPoles * Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(solverSettings.electricPoleType)) + ((solverSettings.numPoles - 1) * gapBetweenElectrifiedAreas);
		ret.second = Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(solverSettings.electricPoleType);
		break;
	case CalculationsUtility::PolesArrangementMethod::RECTANGULAR:
	{
		const unsigned int squareOfPoles = static_cast<unsigned int>(std::sqrt(static_cast<double>(solverSettings.numPoles)));
		ret.first = (squareOfPoles * Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(solverSettings.electricPoleType)) + ((squareOfPoles - 1) * gapBetweenElectrifiedAreas);
		ret.second = ret.first;
		break;
	}
	default:
		IOUtil::Asserts::assertMessageFormatted(false, "CalculationsUtility::Solver::calculateSidesSize - Invalid pole arrangement method = %d", solverSettings.polesArrangementMethod);
	}

	IOUtil::Asserts::assertMessageFormatted((ret.first != 0 && ret.second != 0), "CalculationsUtility::Solver::calculateSidesSize - Invalid size result : X = %d | Y = %d", ret.first, ret.second);
	return ret;
}

void CalculationsUtility::Solver::calculateArrangement(const SolverSettings& solverSettings, std::vector<Entities::SolarPanel> solarPanels, std::vector<Entities::Accumulator> accumulators, std::vector<Entities::ElectricPole*> electricPoles)
{
	//--Delete--
	std::vector<int> uniformRandomContainer;
	RandomUtility::UniformDistribution::getIntegersList(100, uniformRandomContainer, -50, 50);
	//----------

	TilesMapping::ActiveSurfaceMap activeSurfaceMap(calculateSidesSize(solverSettings));
	bool operationSucess = activeSurfaceMap.insertElectricPoles(electricPoles);
	IOUtil::Asserts::assertMessage(operationSucess,"CalculationsUtility::Solver::calculateArrangement - Cannot correctly set Electric Poles in Map");
	if(operationSucess) 
	{
		
		activeSurfaceMap.printSurface();
	}
}
