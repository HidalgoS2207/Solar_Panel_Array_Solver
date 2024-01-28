#include "Solver.h"

void Solver::calculateNumberOfPolesByEffectiveArea(const unsigned int effectiveArea, const Entities::ELECTRIC_POLE_TYPE prefferedElectricPoleType, unsigned int& numPoles)
{
	unsigned int influenceTiles = Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(prefferedElectricPoleType);
	unsigned int occupiedArea = Entities::ElectricPoleAreaOccupiedByType::ElectricPoleAreaOccupied.at(prefferedElectricPoleType);

	double effectiveAreaByPole = static_cast<double>((influenceTiles * influenceTiles) - occupiedArea);

	numPoles = std::ceil(static_cast<double>(effectiveArea) / effectiveAreaByPole);
}
