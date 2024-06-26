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

		modNumPoles = static_cast<unsigned int>(std::pow(squareOfPoles, 2));
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

void CalculationsUtility::Solver::calculateArrangement(
	const SolverSettings& solverSettings
	, std::vector<Entities::SolarPanel*>& solarPanels
	, std::vector<Entities::Accumulator*>& accumulators
	, std::vector<Entities::ElectricPole*>& electricPoles)
{
	TilesMapping::ActiveSurfaceMap activeSurfaceMap(calculateSidesSize(solverSettings));

	auto exportToJsonFile = [](Entities::EntityPtrList& fullEntityList)
		{
			Output::Json json;
			json.saveToFile("bluePrintJsonOutput.txt", fullEntityList);
		};

	if (activeSurfaceMap.insertElectricPoles(electricPoles))
	{
		Entities::EntityPtrList fullEntityList;

		Entities::Entity::insertToEntityPtrList(solarPanels, fullEntityList);
		Entities::Entity::insertToEntityPtrList(accumulators, fullEntityList);
		Entities::Entity::insertToEntityPtrList(electricPoles, fullEntityList);

		switch (solverSettings.entitiesArrangementStrategy)
		{
		case CalculationsUtility::EntityArrangementStrategy::ENHANCED:
		{
			if (calculateArrangementWithEnhancedAlgorithm(solverSettings, activeSurfaceMap, fullEntityList)) { exportToJsonFile(fullEntityList); }
		}
		break;
		default:
			if (calculateArrangementStandard(solverSettings, activeSurfaceMap, solarPanels, accumulators, electricPoles, fullEntityList)) { exportToJsonFile(fullEntityList); }
			break;
		}
	}
	else
	{
		IOUtil::Asserts::assertMessage(sFailure, "CalculationsUtility::Solver::calculateArrangement - Cannot correctly set Electric Poles in Map");
	}
}

bool CalculationsUtility::Solver::calculateArrangementStandard(
	const SolverSettings& solverSettings
	, TilesMapping::ActiveSurfaceMap& activeSurfaceMap
	, std::vector<Entities::SolarPanel*>& solarPanels
	, std::vector<Entities::Accumulator*>& accumulators
	, std::vector<Entities::ElectricPole*>& electricPoles
	, Entities::EntityPtrList& fullEntityList)
{
	/*Debug vars*/
	const bool verboseExecution = true;
	const unsigned int printProgressionVal = 100;
	/*----------*/

	using uintPairCoordinates = std::pair<unsigned int, unsigned int>;

	static unsigned int sMaxIterationsNumber = 100000;

	struct TileInfo
	{
		TileInfo()
		{
			coordinates = { 0,0 };
			isAvailable = false;
		}

		uintPairCoordinates coordinates;
		bool isAvailable;
	};
	using TilesInfoList = std::vector<TileInfo>;
	using TilesInfoByTileCoordinates = std::map<uintPairCoordinates, TileInfo*>;

	auto updateTilesInfo = [](Entities::Entity* entityPtr, TilesInfoList& tilesInfoList, TilesInfoByTileCoordinates& tilesInfoByTileCoordinates, uintPairCoordinates pos)
		{
			uintPairCoordinates entitySize = entityPtr->getTilesDistribution();
			TilesInfoByTileCoordinates::iterator tilesInfoByTileCoordinatesIt;

			for (uint32_t i = 0; i < entitySize.second; i++)
			{
				for (uint32_t j = 0; j < entitySize.first; j++)
				{
					tilesInfoByTileCoordinatesIt = tilesInfoByTileCoordinates.find(pos);
					if (tilesInfoByTileCoordinatesIt != tilesInfoByTileCoordinates.end())
					{
						(tilesInfoByTileCoordinatesIt)->second->isAvailable = false;
					}
					pos.first++;
				}
				pos.second++;
			}
		};

	auto generateTilesInfoList = [&]() -> TilesInfoList
		{
			TilesInfoList tilesInfoList;
			const uintPairCoordinates tilesMapSize = activeSurfaceMap.getTilesMapSize();

			for (uint32_t i = 0; i < tilesMapSize.second; i++)
			{
				for (uint32_t j = 0; j < tilesMapSize.first; j++)
				{
					tilesInfoList.emplace_back();
					tilesInfoList.back().coordinates = { j,i };
					tilesInfoList.back().isAvailable = activeSurfaceMap.getIsAvailable({ j,i });
				}
			}

			return tilesInfoList;
		};

	auto resetTiles = [&](Entities::EntityPtrList& entitiesList, TilesInfoList& tilesInfoList, TilesInfoByTileCoordinates& tilesInfoByTileCoordinates) -> bool
		{
			Entities::Entity::resetEntities(entitiesList);
			activeSurfaceMap.refreshTilesState();
			tilesInfoList.clear();
			tilesInfoList = generateTilesInfoList();
			tilesInfoByTileCoordinates.clear();
			for (TileInfo& tileInfo : tilesInfoList)
			{
				tilesInfoByTileCoordinates[tileInfo.coordinates] = &tileInfo;
			}

			return true;
		};

	auto setGeneralistOrder = [&](Entities::EntityPtrList& entitiesToPlace)
		{
			switch (solverSettings.entitiesSpawnStrategy)
			{
			case CalculationsUtility::EntitySpawnStrategy::FULL_RANDOM:
				RandomUtility::ListOperations::randomizeList(RandomUtility::RandomDistribution::UNIFORM, entitiesToPlace);
				break;
			case CalculationsUtility::EntitySpawnStrategy::WEIGHTED_RANDOM:
				break;
			case CalculationsUtility::EntitySpawnStrategy::FULL_SEQUENTIAL:
				break;
			case CalculationsUtility::EntitySpawnStrategy::WEIGHTED_SEQUENCIAL:
				break;
			default:
				IOUtil::Asserts::assertMessageFormatted(false, "CalculationsUtility::Solver::calculateArrangement - EntitySpawnStrategy <%d> not identified - Using standard list order instead.", solverSettings.entitiesSpawnStrategy);
				break;
			}
		};

	auto setTilesInfoList = [&](TilesInfoList& tilesInfoList)
		{
			switch (solverSettings.entitiesArrangementStrategy)
			{
			case CalculationsUtility::EntityArrangementStrategy::RADIAL_OUT_FIRST:
				break;
			case CalculationsUtility::EntityArrangementStrategy::RADIAL_IN_FIRST:
				break;
			case CalculationsUtility::EntityArrangementStrategy::LINEAR_VER:
				break;
			case CalculationsUtility::EntityArrangementStrategy::LINEAR_HOR:
				//Do nothing an iterate naturally trought the list
				break;
			case CalculationsUtility::EntityArrangementStrategy::ALTERNATE_VER:
				break;
			case CalculationsUtility::EntityArrangementStrategy::ALTERNATE_HOR:
				break;
			default:
			case CalculationsUtility::EntityArrangementStrategy::RANDOM:
				RandomUtility::ListOperations::randomizeList(RandomUtility::RandomDistribution::UNIFORM, tilesInfoList);
				break;
			}
		};

	//----------------------------------------------------------------------
	// ! Rendering
	GFX::Window renderHandler(0.2);

	for (Entities::Entity* entityPtr : fullEntityList)
	{
		const Entities::ENTITY_TYPE entityType = entityPtr->getEntityType();
		const uint32_t entityId = entityPtr->getEntityId();

		switch (entityType)
		{
		case Entities::ENTITY_TYPE::ELECTRIC_POLE:
		{
			const Entities::ELECTRIC_POLE_TYPE electricPoleType = dynamic_cast<Entities::ElectricPole*>(entityPtr)->getElectricPoleType();
			renderHandler.declareRendereable(entityId, entityType, electricPoleType);
		}
		break;
		default:
			renderHandler.declareRendereable(entityId, entityType);
			break;
		}
	}
	//----------------------------------------------------------------------

	Entities::EntityPtrList entitiesToPlace;

	//--------------------------------------------------
	//Rendering
	for (Entities::Entity* entityPtr : electricPoles)
	{
		renderHandler.updateRendereablePosition(entityPtr->getEntityId(), entityPtr->getPosition());
	}
	//--------------------------------------------------

	setEntitiesGeneralList(solarPanels, entitiesToPlace);
	setEntitiesGeneralList(accumulators, entitiesToPlace);

	TilesInfoList tilesInfoList = generateTilesInfoList();
	TilesInfoByTileCoordinates tilesInfoByTileCoordinates;
	for (TileInfo& tileInfo : tilesInfoList)
	{
		tilesInfoByTileCoordinates[tileInfo.coordinates] = &tileInfo;
	}

	unsigned int iteration = 0;
	bool reDistribute = false;

	setGeneralistOrder(entitiesToPlace);
	setTilesInfoList(tilesInfoList);
	do
	{
		reDistribute = false;
		for (Entities::Entity* entityPtr : entitiesToPlace)
		{
			for (TileInfo& tileInfo : tilesInfoList)
			{
				if (!tileInfo.isAvailable) { continue; }

				if (activeSurfaceMap.insertEntity(entityPtr, tileInfo.coordinates))
				{
					updateTilesInfo(entityPtr, tilesInfoList, tilesInfoByTileCoordinates, tileInfo.coordinates);
					// ! Rendering
					renderHandler.updateRendereablePosition(entityPtr->getEntityId(), entityPtr->getPosition());
					break;
				}
			}

			// ! Rendering--------------------------------------
			renderHandler.render();
			//--------------------------------------------------

			if (!entityPtr->getIsPlaced())
			{
				//--------------------------------------------------
				//Rendering
				for (Entities::Entity* entityPtr : fullEntityList)
				{
					renderHandler.resetRendereablePosition(entityPtr->getEntityId());
				}
				//--------------------------------------------------

				reDistribute = resetTiles(entitiesToPlace, tilesInfoList, tilesInfoByTileCoordinates);
				setTilesInfoList(tilesInfoList);
				setGeneralistOrder(entitiesToPlace);
				iteration++;
				IOUtil::Asserts::assertMessageFormatted(!(reDistribute && verboseExecution && !(iteration % printProgressionVal)), "Can't place all entities with the current configuration, resetting tiles map and redistributing.Iteration : %d", iteration);
				break;
			}
		}
	} while ((iteration <= sMaxIterationsNumber) && (reDistribute) && renderHandler.windowState());

	IOUtil::Asserts::assertMessageFormatted(sFailure, "Calculations ended in Iteration : %d", iteration);

	if ((iteration > sMaxIterationsNumber) && (reDistribute)) { return sFailure; }

	const unsigned int freeSurface = activeSurfaceMap.getFreeSurface();
	const unsigned int totalSurface = activeSurfaceMap.getTilesMapSize().first * activeSurfaceMap.getTilesMapSize().second;

	std::cout << "\n\nUtilized space : " << ((1.f - (static_cast<float>(freeSurface) / static_cast<float>(totalSurface))) * 100.f) << "%\n";

	activeSurfaceMap.printSurface();

	return sSuccess;
}

bool CalculationsUtility::Solver::calculateArrangementWithEnhancedAlgorithm(
	  const SolverSettings& solverSettings
	, TilesMapping::ActiveSurfaceMap& activeSurfaceMap
	, Entities::EntityPtrList& entities)
{
	return sFailure;
}
