#pragma once

#include "Entity.h"

#include <SFML/Graphics.hpp>

namespace GFX
{
	class EntityTypeWrapper
	{
	public:
		enum class EntityType
		{
			INVALID = -1,
			SOLAR_PANEL,
			ACCUMULATOR,
			RADAR,
			ROBOPORT,
			SMALL_ELECTRIC_POLE,
			MEDIUM_ELECTRIC_POLE,
			BIG_ELECTRIC_POLE,
			SUBSTATION
		};
	public:
		EntityTypeWrapper(EntityType entityType);
		~EntityTypeWrapper();

		friend bool operator<(const EntityTypeWrapper& entTyWrap1, const EntityTypeWrapper& entTyWrap2)
		{
			return (entTyWrap1.entityType < entTyWrap2.entityType);
		}
	private:
		EntityType entityType;
	};

	class ShapeWrapper
	{
	public:
		enum class ShapeType
		{
			CONVEX_SHAPE,
			CIRCLE_SHAPE,
			RECTANGLE_SHAPE
		};
	public:
		ShapeWrapper(ShapeType shapeType);
		~ShapeWrapper();


		void draw(sf::RenderWindow& renderWindowRef);
	private:
		sf::Shape* shapePtr;
		const ShapeType shapeType;
	};

	using EntityRepresentation = std::vector<ShapeWrapper>;
	using EntitiesRepMapping = std::map<EntityTypeWrapper, EntityRepresentation>;
	using EntityId = uint32_t;

	class Rendereable
	{
	public:
		Rendereable();
		~Rendereable();

		void draw(sf::RenderWindow& renderWindowRef);
	private:
		void setEntityRepresentationInfo();

		EntitiesRepMapping entitiesRepMapping;

		double scaleFactor;
		std::map<EntityId, EntityRepresentation> entities;
	};

	class Window
	{
	public:
		Window();
		~Window();

		void render();
		void declareRendereable(EntityId entityId, Entities::ENTITY_TYPE entityType, Entities::ELECTRIC_POLE_TYPE electricPoleType = Entities::ELECTRIC_POLE_TYPE::INVALID);
	private:
		sf::RenderWindow* renderWindow;
		Rendereable renderableObjets;
	};
}