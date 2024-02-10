#pragma once

#include "ActiveSurfaceMap.h"
#include "Entity.h"

#include <algorithm>
#include <iostream>
#include <math.h>
#include <random>
#include <stdarg.h>
#include <vector>

namespace IOUtil
{
	class KbdIO
	{
	private:
		static inline bool verifyInputRange(int min, int max, int val) { return (val <= max && val >= min); };
	public:
		static inline int userInputPrompt(const char* promptText, const char* errorText, const std::pair<int, int> inputRange, int inputRangeOffset)
		{
			//! First loop always false
			int ret = inputRange.first - 1;

			while (!verifyInputRange(inputRange.first, inputRange.second, ret))
			{
				std::cout << promptText;
				std::cin >> ret;
				if (!verifyInputRange(inputRange.first, inputRange.second, ret))
				{
					std::cout << errorText;
				}
			}

			return (ret + inputRangeOffset);
		};

		static inline int userInputPrompt(const char* prompTitle, const std::vector<std::string> userOptions, const char* errorText, const std::pair<int, int> inputRange)
		{
			auto printUserOptions = [&]()
				{
					std::cout << "\n";

					for (int i = 0; i < userOptions.size(); i++)
					{
						std::cout << (i + 1) << ". " << userOptions[i] << "\n";
					}

					std::cout << "> ";
				};

			const int offSet = inputRange.first - 1;

			//! First loop always false
			int ret = inputRange.first - offSet - 1;

			while (!verifyInputRange(inputRange.first - offSet, inputRange.second - offSet, ret))
			{
				std::cout << "\n" << prompTitle;
				printUserOptions();
				std::cin >> ret;
				if (!verifyInputRange(inputRange.first - offSet, inputRange.second - offSet, ret))
				{
					std::cout << errorText;
				}
			}

			return (ret + offSet);
		};
	};

	class Asserts
	{
	private:
		enum class FORMATTED_TYPES
		{
			INVALID = -1,
			INTEGER,
			FLOAT,
			CHAR,
			STRING
		};
	public:
		static inline void assertMessage(const bool condition, const char* msg)
		{
			if (!condition)
			{
				std::cout << "\n" << msg << "\n";
			}
		}

		//! Print formatted message in console output [%d = integer | %f = float | %s = string | %c = character]
		static inline void assertMessageFormatted(const bool condition, const char* msg, ...)
		{
			if (!condition)
			{
				std::cout << "\n";

				va_list args;
				va_start(args, msg);

				auto identyType = [&](const char* strPos)
					{
						switch (*strPos)
						{
						case 'd':
							std::cout << va_arg(args, int);
							break;
						case 'f':
							std::cout << va_arg(args, float);
							break;
						case 'c':
							std::cout << va_arg(args, char);
							break;
						case 's':
							std::cout << va_arg(args, char*);
							break;
						default:
							break;
						}
					};

				auto identifyArg = [](const char* strPos) -> bool
					{
						if (*(strPos + 1) != '\0')
						{
							return ((*(strPos + 1) == 'd') || (*(strPos + 1) == 'f') || (*(strPos + 1) == 's') || (*(strPos + 1) == 'c'));
						}

						return false;
					};

				while (*msg != '\0')
				{
					if (*msg == '%')
					{
						if (identifyArg(msg))
						{
							msg++;
							identyType(msg);
						}
						else
						{
							std::cout << *msg;
						}
					}
					else
					{
						std::cout << *msg;
					}
					msg++;
				}

				va_end(args);

				std::cout << "\n";
			}

		}
	};
}

namespace RandomUtility
{
	enum RandomDistribution
	{
		UNIFORM,
		NORMAL
	};

	static class UniformDistribution
	{
	public:
		static void getIntegersList(const unsigned int retSize, std::vector<int>& container, int min, int max)
		{
			std::random_device r;

			std::default_random_engine e1(r());
			std::uniform_int_distribution<int> uniform_dist(min, max);

			for (int i = 0; i < retSize; i++)
			{
				container.push_back(uniform_dist(e1));
			}
		}

		static int getIntegerValue(int min, int max)
		{
			std::vector<int> retCont;
			getIntegersList(1, retCont, min, max);
			return retCont.front();
		}
	};

	static class ListOperations
	{
	public:
		template<typename T>
		static void randomizeList(RandomDistribution distributionType, std::vector<T>& list)
		{
			switch (distributionType)
			{
			case RandomUtility::UNIFORM:
			{
				std::vector<int> listOfRandomIdx;
				UniformDistribution::getIntegersList(list.size(), listOfRandomIdx, 0, list.size() - 1);
				std::vector<int>::iterator listOfRandomIdxIt = listOfRandomIdx.begin();
				for (auto& listElem : list)
				{
					std::swap(listElem, list[*listOfRandomIdxIt++]);
				}
			}
			break;
			case RandomUtility::NORMAL:
				break;
			default:
				break;
			}
		}
	};
}

namespace CalculationsUtility
{
	const double AccumulatorToSolarPanelsRatio = 0.84;

	enum class PolesArrangementMethod
	{
		INVALID = -1,
		LINEAR,
		RECTANGULAR
	};

	enum class EntitySpawnStrategy
	{
		INVALID = -1,
		FULL_RANDOM,
		WEIGHTED_RANDOM,
		FULL_SEQUENTIAL,
		WEIGHTED_SEQUENCIAL
	};

	enum class EntityArrangementStrategy
	{
		INVALID = -1,
		RADIAL_IN_FIRST,
		RADIAL_OUT_FIRST,
		LINEAR_HOR,
		LINEAR_VER,
		RANDOM,
		ALTERNATE_VER,
		ALTERNATE_HOR
	};

	struct SolverSettings
	{
	public:
		SolverSettings()
		{
			initializeData();
		}

		void resetSettingsData()
		{
			initializeData();
		}

		PolesArrangementMethod polesArrangementMethod;
		EntitySpawnStrategy entitiesSpawnStrategy;
		EntityArrangementStrategy entitiesArrangementStrategy;
		Entities::ELECTRIC_POLE_TYPE electricPoleType;
		unsigned int numPoles;
		unsigned int numSolarPanels;
		unsigned int numAccumulators;

	private:
		void initializeData()
		{
			polesArrangementMethod = PolesArrangementMethod::INVALID;
			entitiesSpawnStrategy = EntitySpawnStrategy::INVALID;
			entitiesArrangementStrategy = EntityArrangementStrategy::INVALID;
			electricPoleType = Entities::ELECTRIC_POLE_TYPE::INVALID;
			numPoles = 0;
			numSolarPanels = 0;
			numAccumulators = 0;
		}
	};

	class Solver
	{
	private:
		template<typename E>
		static inline void setEntitiesGeneralList(std::vector<E>& listIn, std::vector<Entities::Entity*>& listOut)
		{
			for (E& elemIn : listIn)
			{
				listOut.push_back(dynamic_cast<Entities::Entity*>(&elemIn));
			}
		}

		static inline unsigned int calculateMaxDistanceBetweenPoles(Entities::ELECTRIC_POLE_TYPE electricPoleType, unsigned int& gapBetweenElectrifiedAreas)
		{
			gapBetweenElectrifiedAreas = (Entities::maxGapBetweenPolesInfluenceArea);

			unsigned int maxDistance = static_cast<int>((Entities::ElectricPoleInfluenceTilesByType::ElectricPoleInfluence.at(electricPoleType))) + (Entities::maxGapBetweenPolesInfluenceArea);

			while (maxDistance > Entities::ElectricPoleWireTilesDistanceByType::ElectricPoleWireTilesDistance.at(electricPoleType))
			{
				//Reduce by 1 until we reach the maximum possible distance
				maxDistance--;
				gapBetweenElectrifiedAreas--;
			}

			return maxDistance;
		}
	public:
		static inline unsigned int calculateMaxDistanceBetweenPoles(Entities::ELECTRIC_POLE_TYPE electricPoleType)
		{
			unsigned int gapBetweenElectrifiedAreasIgnoreByRef = 0;
			return calculateMaxDistanceBetweenPoles(electricPoleType, gapBetweenElectrifiedAreasIgnoreByRef);
		}

		static inline unsigned int calculateGapBetweenElectrifiedAreas(Entities::ELECTRIC_POLE_TYPE electricPoleType)
		{
			unsigned int gapBetweenElectrifiedAreas = 0;
			calculateMaxDistanceBetweenPoles(electricPoleType, gapBetweenElectrifiedAreas);
			return gapBetweenElectrifiedAreas;
		}

		template<typename T>
		static void instantiateEntities(unsigned int numEntities, std::vector<T>& entities)
		{
			for (unsigned int i = 0; i < numEntities; i++)
			{
				entities.emplace_back();
			}
		}

		static void instantiateEntities(const unsigned int numEntities, std::vector<Entities::ElectricPole*>& electricPoles, const Entities::ELECTRIC_POLE_TYPE electricPoleType)
		{
			auto allocateElectricPoleByType = [](const Entities::ELECTRIC_POLE_TYPE electricPoleType) -> Entities::ElectricPole*
				{
					switch (electricPoleType)
					{
					case Entities::ELECTRIC_POLE_TYPE::SMALL:
					{
						Entities::ElectricPole* electricPole = new Entities::SmallElectricPole();
						return electricPole;
					}
					case Entities::ELECTRIC_POLE_TYPE::MEDIUM:
					{
						Entities::ElectricPole* electricPole = new Entities::MediumElectricPole();
						return electricPole;
					}
					case Entities::ELECTRIC_POLE_TYPE::BIG:
					{
						Entities::ElectricPole* electricPole = new Entities::BigElectricPole();
						return electricPole;
					}
					case Entities::ELECTRIC_POLE_TYPE::SUBSTATION:
					{
						Entities::ElectricPole* electricPole = new Entities::SubStation();
						return electricPole;
					}
					default:
						IOUtil::Asserts::assertMessageFormatted(false, "instantiateEntities - Cannot initialize Electric Pole Instance of type Id: %d", electricPoleType);
						break;
					}

					return nullptr;
				};

			for (unsigned int i = 0; i < numEntities; i++)
			{
				electricPoles.push_back(allocateElectricPoleByType(electricPoleType));
			}
		}

		static void destroyEntities(std::vector<Entities::ElectricPole*>& electricPoles)
		{
			for (auto& electricPole : electricPoles)
			{
				delete electricPole;
			}
		}

		template<typename T>
		static unsigned int calculateOccupiedSurface(std::vector<T>& entities)
		{
			unsigned int ret = 0;
			for (T& entity : entities)
			{
				const std::pair<unsigned int, unsigned int>& tilesDist = entity.getTilesDistribution();

				ret += (tilesDist.first * tilesDist.second);
			}

			return ret;
		}

		static unsigned int calculatePotentialMaxEffectiveArea(const SolverSettings& solverSettings, unsigned int& effectiveArea);
		static std::pair<unsigned int, unsigned int> calculateSidesSize(const SolverSettings& solverSettings);
		static void calculateArrangement(const SolverSettings& solverSettings, std::vector<Entities::SolarPanel> solarPanels, std::vector<Entities::Accumulator> accumulators, std::vector<Entities::ElectricPole*> electricPoles);
	};
}

