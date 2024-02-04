#pragma once

#include "Entity.h"

#include <iostream>
#include <math.h>
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
						bool ret = (*(strPos + 1) == 'd') || (*(strPos + 1) == 'f') || (*(strPos + 1) == 's') || (*(strPos + 1) == 'c');
						return ret;
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

		static unsigned int calculatePotentialMaxEffectiveArea(const SolverSettings& solverSettings, unsigned int& effectiveArea);
		static void calculateArrangement(const SolverSettings& solverSettings);
	};
}

