#include "Util.h"

void CalculationsUtility::Solver::calculatePotentialMaxEffectiveArea(SolverSettings& solverSettings, unsigned int& effectiveArea)
{
	const unsigned int influenceTiles = Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(solverSettings.electricPoleType);
	const double wireDistance = Entities::ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(solverSettings.electricPoleType);

	unsigned int maxDistance = Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(solverSettings.electricPoleType) + (Entities::maxGapBetweenPolesInflueceArea);

	if (maxDistance > Entities::ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(solverSettings.electricPoleType))
	{
		maxDistance = Entities::ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(solverSettings.electricPoleType);
	}

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
	}
	break;
	}
}

void CalculationsUtility::Solver::calculateArrangement(const SolverSettings solverSettings)
{

}
