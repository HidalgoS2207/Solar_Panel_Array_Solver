#pragma once

#include <vector>

namespace Entities
{
	enum ELECTRIC_POLE_TYPE
	{
		INVALID = 0,
		SMALL = 1,
		MEDIUM,
		BIG,
		SUBSTATION
	};

	class Entity
	{
	public:
		virtual ~Entity();

		const std::pair<unsigned int, unsigned int>& getTilesDistribution() const;
		const std::pair<unsigned int, unsigned int>& getPosition() const;

		void setPosition(std::pair<unsigned int, unsigned int> newPosition);
	protected:
		Entity(const std::vector<bool> tilesMap, std::pair<unsigned int, unsigned int > tilesDistribution, std::pair<unsigned int, unsigned int > position);
	private:
		unsigned int tiles;
		std::vector<bool> tilesDistMap;
		std::pair<unsigned int, unsigned int> tilesDistribution;
		std::pair<unsigned int, unsigned int> position;
	};

	class SolarPanel : public Entity
	{
	public:
		SolarPanel();
		~SolarPanel();
	private:
	};

	class ElectricPole : public Entity
	{
	public:
		~ElectricPole();

		unsigned int getInfluenceTiles();
		double getWireTilesDistance();
	protected:
		ElectricPole(const std::vector<bool> tilesMap, std::pair<unsigned int, unsigned int > tilesDistribution, unsigned int influenceTiles, double wireTilesDistance, ELECTRIC_POLE_TYPE electricPoleType);
	private:
		const unsigned int influenceTiles;
		const double wireTilesDistance;
		const ELECTRIC_POLE_TYPE electricPoleType;
	};

	class SmallElectricPole : public ElectricPole
	{
	public:
		SmallElectricPole();
		~SmallElectricPole();
	private:
	};

	class MediumElectricPole : public ElectricPole
	{
	public:
		MediumElectricPole();
		~MediumElectricPole();
	private:
	};

	class BigElectricPole : public ElectricPole
	{
	public:
		BigElectricPole();
		~BigElectricPole();
	private:
	};

	class SubStation : public ElectricPole
	{
	public:
		SubStation();
		~SubStation();
	private:
	};

	class Accumulator : public Entity
	{
	public:
		Accumulator();
		~Accumulator();
	private:
	};
}

