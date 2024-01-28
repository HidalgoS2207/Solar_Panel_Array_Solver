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
}

namespace CalculationsUtility
{
	const double AccumulatorToSolarPanelsRatio = 0.84;

	enum PolesArrangementMethod
	{
		LINEAR,
		RECTANGULAR
	};

	class Solver
	{
	public:
		template<typename T>
		static void instantiateEntities(unsigned int numEntities, std::vector<T>& entities)
		{
			for (int i = 0; i < numEntities; i++)
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

		static void calculatePotentialMaxEffectiveArea(const Entities::ELECTRIC_POLE_TYPE prefferedElectricPoleType,const PolesArrangementMethod polesArrangementMethod, const unsigned int numPoles, unsigned int& effectiveArea);
	};
}

