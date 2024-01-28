#include "Util.h"

void CalculationsUtility::Solver::calculatePotentialMaxEffectiveArea(const Entities::ELECTRIC_POLE_TYPE prefferedElectricPoleType, const PolesArrangementMethod polesArrangementMethod, const unsigned int numPoles, unsigned int& effectiveArea)
{
	unsigned int influenceTiles = Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(prefferedElectricPoleType);
	const double wireDistance = Entities::ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(prefferedElectricPoleType);

	unsigned int maxDistance = Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(prefferedElectricPoleType) + (Entities::maxGapBetweenPolesInflueceArea);

	if (maxDistance > Entities::ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(prefferedElectricPoleType))
	{
		maxDistance = Entities::ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(prefferedElectricPoleType);
	}

	switch (polesArrangementMethod)
	{
	case CalculationsUtility::LINEAR:
	{
		for (unsigned int i = 0; i < numPoles; i++)
		{
			effectiveArea += ((maxDistance * Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(prefferedElectricPoleType)) - Entities::ElectricPoleAreaOccupiedByType::ElectricPoleAreaOccupied.at(prefferedElectricPoleType));
		}
	}
	break;
	case CalculationsUtility::RECTANGULAR:
	{
		const unsigned int squareOfPoles = static_cast<unsigned int>(std::ceil(std::sqrt(static_cast<double>(numPoles))));

		for (unsigned int i = 0; i < squareOfPoles; i++)
		{
			for (unsigned int i = 0; i < squareOfPoles; i++)
			{
				effectiveArea += ((maxDistance * Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(prefferedElectricPoleType)) - Entities::ElectricPoleAreaOccupiedByType::ElectricPoleAreaOccupied.at(prefferedElectricPoleType));
			}

			effectiveArea += ((maxDistance - Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(prefferedElectricPoleType)) * Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(prefferedElectricPoleType));
		}
	}
	break;
	}
}
