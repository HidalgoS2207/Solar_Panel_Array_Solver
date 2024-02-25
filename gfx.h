#pragma once

#include "Entity.h"
#include "Util.h"

#include <SFML/Graphics.hpp>

namespace GFX
{
	using uIntPair = std::pair<unsigned int, unsigned int>;
	using floatPair = std::pair<float, float>;

	using EntityId = uint32_t;

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
		EntityTypeWrapper(const EntityTypeWrapper& entityTypeWrapper);
		~EntityTypeWrapper();

		const floatPair& getPosition();
		void setPosition(floatPair position);

		const EntityTypeWrapper::EntityType getEntityType() const;

		friend bool operator<(const EntityTypeWrapper& entTyWrap1, const EntityTypeWrapper& entTyWrap2)
		{
			return (entTyWrap1.entityType < entTyWrap2.entityType);
		}
	private:
		EntityType entityType;

		floatPair position;
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
		ShapeWrapper(const ShapeWrapper& shapeWrapperRef);
		~ShapeWrapper();

		void setShapeInfo(const sf::Color shapeColor, const uIntPair relPos, const floatPair size);

		void draw(sf::RenderWindow& renderWindowRef);
	private:
		sf::Shape* shapePtr;
		const ShapeType shapeType;
		uIntPair relPos;
	};

	using EntityRepresentation = std::vector<ShapeWrapper*>;
	using EntityTypeWrapperByEntityId = std::map<EntityId, EntityTypeWrapper>;
	using EntityRepresentationByEntityTypeWrapper = std::map<EntityTypeWrapper::EntityType, EntityRepresentation>;

	class EntitiesRepMapping
	{
	public:
		EntitiesRepMapping();
		~EntitiesRepMapping();

		EntityRepresentation& getEntityRep(const EntityTypeWrapper& entityType);
	private:
		void setEntityRepresentationInfo();

		EntityRepresentationByEntityTypeWrapper EntityRepresentationByEntityType;
	};

	class Rendereable
	{
	public:
		Rendereable();
		~Rendereable();

		void draw(sf::RenderWindow& renderWindowRef);
		void insert(const EntityId entityId, const EntityTypeWrapper entityTypeWrapper);

		const EntityTypeWrapper getEntityTypeWrapper(Entities::ENTITY_TYPE entityType, Entities::ELECTRIC_POLE_TYPE electricPoleType = Entities::ELECTRIC_POLE_TYPE::INVALID);
	private:
		EntitiesRepMapping entitiesRepMapping;

		double scaleFactor;
		EntityTypeWrapperByEntityId entityTypeWrapperByEntityId;
	};

	class Window
	{
	public:
		Window();
		~Window();

		void render();
		void handleEvents();
		bool windowState();
		void declareRendereable(EntityId entityId, Entities::ENTITY_TYPE entityType, Entities::ELECTRIC_POLE_TYPE electricPoleType = Entities::ELECTRIC_POLE_TYPE::INVALID);
	private:
		sf::RenderWindow* renderWindow;
		sf::Event event;
		Rendereable renderableObjets;
	};
}