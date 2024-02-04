#pragma once

#include "Entity.h"

#include <iostream>
#include <vector>
#include <math.h>

namespace IOUtil
{
	class KbdIO
	{
	private:
		static inline bool verifyInputRange(int min, int max, int val) { return (val <= max && val >= min); };
	public:
		static inline int userInputPrompt(const char* promptText, const char* errorText, const std::pair<int, int> inputRange, int inputRangeOffset)
		{
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
	};

	class Asserts
	{
	private:
	public:
		static inline void assertMessage(const bool condition, const char* msg)
		{
			if (!condition)
			{
				std::cout << "\n" << msg << "\n";
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
	public:
		template<typename T>
		static void instantiateEntities(unsigned int numEntities, std::vector<T>& entities)
		{
			for (unsigned int i = 0; i < numEntities; i++)
			{
				entities.emplace_back();
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

		static void calculatePotentialMaxEffectiveArea(SolverSettings& solverSettings, unsigned int& effectiveArea);
		static void calculateArrangement(const SolverSettings solverSettings);
	};
}

