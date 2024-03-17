#pragma once

#include "Entity.h"
#include "Util.h"

#include <chrono>
#include <numeric>

#include <SFML/Graphics.hpp>

namespace GFX
{
	using uIntPair = std::pair<unsigned int, unsigned int>;
	using floatPair = std::pair<float, float>;

	using EntityId = uint32_t;
	
	using PointsList = std::vector<floatPair>;

	static const bool verboseExecution = false;

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
		void setPosition(floatPair position, const bool isVisible = true);
		void resetPosition();

		const EntityTypeWrapper::EntityType getEntityType() const;

		friend bool operator<(const EntityTypeWrapper& entTyWrap1, const EntityTypeWrapper& entTyWrap2)
		{
			return (entTyWrap1.entityType < entTyWrap2.entityType);
		}

		const bool getIsVisible() const;
	private:
		EntityType entityType;

		floatPair position;

		bool isVisible;
	};

	class ShapeWrapper
	{
	public:
		enum class ShapeType
		{
			CONVEX_SHAPE,
			CIRCLE_SHAPE,
			RECTANGLE_SHAPE,
			TEXT
		};
	public:
		ShapeWrapper(ShapeType shapeType);
		ShapeWrapper(const ShapeWrapper& shapeWrapperRef);
		~ShapeWrapper();

		void setShapeInfo(const sf::Color shapeColor, const floatPair relPos, const floatPair size);
		void setShapeInfo(const sf::Color shapeColor, const floatPair relPos, const float radius);
		void setShapeInfo(const sf::Color shapeColor, const floatPair relPos, const PointsList pointsList);
		void setShapeInfo(const sf::Color shapeColor, const floatPair relPos, const char* textToDisplay, const unsigned int fontSize = sInitialFontSize);

		void setPosition(const floatPair absPosition = {0.0,0.0});
		void resetPosition();

		void draw(sf::RenderWindow& renderWindowRef);
	private:
		sf::Shape* shapePtr;
		sf::Text* textPtr;
		sf::Font font;
		const ShapeType shapeType;
		floatPair relPos;

		static const char* sFontFilePath;
		static const unsigned int sInitialFontSize;
	};

	using EntityRepresentation = std::vector<ShapeWrapper*>;
	using EntityTypeWrapperByEntityId = std::map<EntityId, EntityTypeWrapper>;
	using EntityRepresentationByEntityType = std::map<EntityTypeWrapper::EntityType, EntityRepresentation>;

	class EntitiesRepMapping
	{
	private:
	public:
		EntitiesRepMapping();
		~EntitiesRepMapping();

		EntityRepresentation& getEntityRep(const EntityTypeWrapper& entityType);

		floatPair entityPositionToTilePosition(const floatPair position);
	private:
		floatPair entityTypeSizeConverter(const GFX::EntityTypeWrapper::EntityType entityType);

		void setEntityRepresentationInfo();

		EntityRepresentationByEntityType EntityRepresentationByEntityType;

		static const float sPixelsPerTile;
	};

	class Rendereable
	{
	public:
		Rendereable();
		~Rendereable();

		void draw(sf::RenderWindow& renderWindowRef);
		void insert(const EntityId entityId, const EntityTypeWrapper entityTypeWrapper);
		void updateEntityPosition(const EntityId entityId, const floatPair pos, const bool isVisible = true);
		void resetEntityPosition(const EntityId entityId);

		const EntityTypeWrapper getEntityTypeWrapper(Entities::ENTITY_TYPE entityType, Entities::ELECTRIC_POLE_TYPE electricPoleType = Entities::ELECTRIC_POLE_TYPE::INVALID);
	private:
		EntitiesRepMapping entitiesRepMapping;

		double scaleFactor;
		EntityTypeWrapperByEntityId entityTypeWrapperByEntityId;
	};

	class Window
	{
	public:
		Window(const double renderDelaySeconds);
		~Window();

		void render(const double currentDelayCountSeconds = std::numeric_limits<double>::max());
		void updateRendereablePosition(const EntityId id,const floatPair pos);
		void resetRendereablePosition(const EntityId id);
		void handleEvents();
		bool windowState();
		void declareRendereable(EntityId entityId, Entities::ENTITY_TYPE entityType, Entities::ELECTRIC_POLE_TYPE electricPoleType = Entities::ELECTRIC_POLE_TYPE::INVALID);
	
	private:
		sf::RenderWindow* renderWindow;
		sf::Event event;
		Rendereable renderableObjets;

		std::chrono::steady_clock::time_point instantTime;
		std::chrono::steady_clock::time_point t1;
		std::chrono::steady_clock::time_point t2;
		std::chrono::nanoseconds t_diff;
		std::chrono::nanoseconds delayCount;
		const double nanosecsPerSecond;
		const double renderDelayNanoSeconds;
		const double BASE_FPS;
		const double FPS;
		const double NANOS_PER_FRAME;
	};
}