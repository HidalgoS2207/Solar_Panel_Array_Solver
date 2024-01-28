#include "Util.h"

void CalculationsUtility::Solver::calculatePotentialMaxEffectiveArea(const Entities::ELECTRIC_POLE_TYPE prefferedElectricPoleType, const PolesArrangementMethod polesArrangementMethod, const unsigned int numPoles, unsigned int& effectiveArea)
{
	//This constant should allow to separate influence areas without the risk of leaving a one tile gap with no entities
	const unsigned int maxGap = (Entities::SolarPanelSideNumTiles * 2) - 3;

	unsigned int influenceTiles = Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(prefferedElectricPoleType);
	unsigned int wireDistance = Entities::ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(prefferedElectricPoleType);


}
