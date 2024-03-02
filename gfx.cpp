#include "gfx.h"

GFX::Window::Window()
{
	renderWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "Enhanced Packing Algorithm Visualization");
}

GFX::Window::~Window()
{
	delete renderWindow;
}

void GFX::Window::render()
{
	renderWindow->clear();

	renderableObjets.draw(*renderWindow);

	renderWindow->display();
}

void GFX::Window::updateRendereablePosition(const EntityId id, const floatPair pos)
{
	renderableObjets.updateEntityPosition(id, pos);
}

void GFX::Window::handleEvents()
{
	while (renderWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			renderWindow->close();
		}
	}
}

bool GFX::Window::windowState()
{
	return renderWindow->isOpen();
}

void GFX::Window::declareRendereable(EntityId entityId, Entities::ENTITY_TYPE entityType, Entities::ELECTRIC_POLE_TYPE electricPoleType)
{
	EntityTypeWrapper entityTypeWrapper = renderableObjets.getEntityTypeWrapper(entityType, electricPoleType);
	renderableObjets.insert(entityId, entityTypeWrapper);
}

GFX::Rendereable::Rendereable()
	:
	scaleFactor(1.f)
{}

GFX::Rendereable::~Rendereable()
{
}

void GFX::Rendereable::draw(sf::RenderWindow& renderWindowRef)
{
	for (auto& entity : entityTypeWrapperByEntityId)
	{
		EntityRepresentation& entityRepList = entitiesRepMapping.getEntityRep(entity.second);
		for (auto entityRepElem : entityRepList)
		{
			entityRepElem->setPosition(entity.second.getPosition());
			entityRepElem->draw(renderWindowRef);
		}
	}
}

void GFX::Rendereable::insert(const EntityId entityId, const EntityTypeWrapper entityTypeWrapper)
{
	entityTypeWrapperByEntityId.insert({ entityId,entityTypeWrapper });
}

void GFX::Rendereable::updateEntityPosition(const EntityId entityId, const floatPair pos)
{
	EntityTypeWrapperByEntityId::iterator entityTypeWrapperIt = entityTypeWrapperByEntityId.find(entityId);

	if (entityTypeWrapperIt == entityTypeWrapperByEntityId.end())
	{
		IOUtil::Asserts::assertMessage(false, "GFX::Rendereable::updateEntityPosition - Position cannot be updated entityId couldn't be found.");
		return;
	}

	entityTypeWrapperIt->second.setPosition(pos);
}

const GFX::EntityTypeWrapper GFX::Rendereable::getEntityTypeWrapper(Entities::ENTITY_TYPE entityType, Entities::ELECTRIC_POLE_TYPE electricPoleType)
{
	switch (entityType)
	{
	case Entities::ENTITY_TYPE::SOLAR_PANEL:
		return EntityTypeWrapper(GFX::EntityTypeWrapper::EntityType::SOLAR_PANEL);
		break;
	case Entities::ENTITY_TYPE::ACCUMULATOR:
		return EntityTypeWrapper(GFX::EntityTypeWrapper::EntityType::ACCUMULATOR);
		break;
	case Entities::ENTITY_TYPE::ELECTRIC_POLE:
	{
		switch (electricPoleType)
		{
		case Entities::SMALL:
			return EntityTypeWrapper(GFX::EntityTypeWrapper::EntityType::SMALL_ELECTRIC_POLE);
			break;
		case Entities::MEDIUM:
			return EntityTypeWrapper(GFX::EntityTypeWrapper::EntityType::MEDIUM_ELECTRIC_POLE);
			break;
		case Entities::BIG:
			return EntityTypeWrapper(GFX::EntityTypeWrapper::EntityType::BIG_ELECTRIC_POLE);
			break;
		case Entities::SUBSTATION:
			return EntityTypeWrapper(GFX::EntityTypeWrapper::EntityType::SUBSTATION);
			break;
		case Entities::INVALID:
		default:
			break;
		}
	}
	break;
	case Entities::ENTITY_TYPE::ROBOPORT:
		return EntityTypeWrapper(GFX::EntityTypeWrapper::EntityType::ROBOPORT);
		break;
	case Entities::ENTITY_TYPE::RADAR:
		return EntityTypeWrapper(GFX::EntityTypeWrapper::EntityType::RADAR);
		break;
	default:
		break;
	}
}

GFX::ShapeWrapper::ShapeWrapper(ShapeType shapeType)
	:
	shapeType(shapeType)
{
	switch (shapeType)
	{
	case GFX::ShapeWrapper::ShapeType::CIRCLE_SHAPE:
		shapePtr = new sf::CircleShape();
		break;
	case GFX::ShapeWrapper::ShapeType::CONVEX_SHAPE:
		shapePtr = new sf::ConvexShape();
		break;
	case GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE:
	default:
		shapePtr = new sf::RectangleShape();
		break;
	}
}

GFX::ShapeWrapper::ShapeWrapper(const ShapeWrapper& shapeWrapperRef)
	:
	shapeType(shapeWrapperRef.shapeType)
{
	this->shapePtr = shapeWrapperRef.shapePtr;
	this->relPos = shapeWrapperRef.relPos;
}

GFX::ShapeWrapper::~ShapeWrapper()
{
	delete shapePtr;
}

void GFX::ShapeWrapper::setShapeInfo(const sf::Color shapeColor, const uIntPair relPos, const floatPair size)
{
	this->relPos = relPos;

	if (this->shapeType == GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE)
	{
		sf::RectangleShape* rectShape = dynamic_cast<sf::RectangleShape*>(shapePtr);

		rectShape->setSize(sf::Vector2f({ size.first,size.second }));
		rectShape->setFillColor(sf::Color::Transparent);
		rectShape->setOutlineColor(shapeColor);
		rectShape->setOutlineThickness(2.0);
	}
	else
	{
		IOUtil::Asserts::assertMessage(false, "GFX::ShapeWrapper::setShapeInfo - Cannot set shape properties. Parameters doesn't match the shape type. Expected RECTANGLE_SHAPE.");
	}
}

void GFX::ShapeWrapper::setShapeInfo(const sf::Color shapeColor, const uIntPair relPos, const float radius)
{
	this->relPos = relPos;

	if (this->shapeType == GFX::ShapeWrapper::ShapeType::CIRCLE_SHAPE)
	{
		sf::CircleShape* circleShape = dynamic_cast<sf::CircleShape*>(shapePtr);

		circleShape->setRadius(radius);
		circleShape->setFillColor(sf::Color::Transparent);
		circleShape->setOutlineColor(shapeColor);
		circleShape->setOutlineThickness(2.0);
	}
	else
	{
		IOUtil::Asserts::assertMessage(false, "GFX::ShapeWrapper::setShapeInfo - Cannot set shape properties. Parameters doesn't match the shape type. Expected CIRCLE_SHAPE.");
	}
}

void GFX::ShapeWrapper::setShapeInfo(const sf::Color shapeColor, const uIntPair relPos, const PointsList pointsList)
{
	this->relPos = relPos;

	if (this->shapeType == GFX::ShapeWrapper::ShapeType::CONVEX_SHAPE)
	{
		sf::ConvexShape* convexShape = dynamic_cast<sf::ConvexShape*>(shapePtr);
		convexShape->setFillColor(sf::Color::Transparent);
		convexShape->setOutlineColor(shapeColor);
		convexShape->setOutlineThickness(2.0);

		int idx = 0;
		convexShape->setPointCount(pointsList.size());
		for (const floatPair& pos : pointsList)
		{
			convexShape->setPoint(idx, { pos.first ,pos.second });
			idx++;
		}
	}
	else
	{
		IOUtil::Asserts::assertMessage(false, "GFX::ShapeWrapper::setShapeInfo - Cannot set shape properties. Parameters doesn't match the shape type. Expected CIRCLE_SHAPE.");
	}
}

void GFX::ShapeWrapper::setPosition(const floatPair absPosition)
{
	shapePtr->setPosition({ absPosition.first + relPos.first,absPosition.second + relPos.second });
}

void GFX::ShapeWrapper::draw(sf::RenderWindow& renderWindowRef)
{
	renderWindowRef.draw(*shapePtr);
}

GFX::EntityTypeWrapper::EntityTypeWrapper(EntityType entityType)
	:
	entityType(entityType),
	position(0.0, 0.0)
{}

GFX::EntityTypeWrapper::EntityTypeWrapper(const EntityTypeWrapper& entityTypeWrapper)
	:
	entityType(entityTypeWrapper.entityType)
{}

GFX::EntityTypeWrapper::~EntityTypeWrapper()
{}

const GFX::floatPair& GFX::EntityTypeWrapper::getPosition()
{
	return position;
}

void GFX::EntityTypeWrapper::setPosition(floatPair position)
{
	this->position = position;
}

const GFX::EntityTypeWrapper::EntityType GFX::EntityTypeWrapper::getEntityType() const
{
	return this->entityType;
}

GFX::EntitiesRepMapping::EntitiesRepMapping()
	:
	pixelsPerTile(20)
{
	setEntityRepresentationInfo();
}

GFX::EntitiesRepMapping::~EntitiesRepMapping()
{
	for (auto& entityRepPair : EntityRepresentationByEntityType)
	{
		for (ShapeWrapper* entityRepElem : entityRepPair.second)
		{
			delete entityRepElem;
		}
	}
}

GFX::EntityRepresentation& GFX::EntitiesRepMapping::getEntityRep(const EntityTypeWrapper& entityType)
{
	return EntityRepresentationByEntityType[entityType.getEntityType()];
}

GFX::floatPair GFX::EntitiesRepMapping::entityTypeSizeConverter(const GFX::EntityTypeWrapper::EntityType entityType)
{
	auto getSideOccupiesTilesByElectricPoleType = [](const Entities::ELECTRIC_POLE_TYPE electricPoleType) -> const int
		{
			return static_cast<int>(std::sqrt(Entities::ElectricPoleAreaOccupiedByType::ElectricPoleAreaOccupied.at(electricPoleType)));
		};

	switch (entityType)
	{
	case GFX::EntityTypeWrapper::EntityType::SOLAR_PANEL:
		return { Entities::SolarPanelSideNumTiles * pixelsPerTile,Entities::SolarPanelSideNumTiles * pixelsPerTile };
		break;
	case GFX::EntityTypeWrapper::EntityType::ACCUMULATOR:
		return { Entities::AccumulatorSideNumTiles * pixelsPerTile,Entities::AccumulatorSideNumTiles * pixelsPerTile };
		break;
	case GFX::EntityTypeWrapper::EntityType::SMALL_ELECTRIC_POLE:
	{
		const int sideOccupiedTiles = getSideOccupiesTilesByElectricPoleType(Entities::ELECTRIC_POLE_TYPE::SMALL);
		return { sideOccupiedTiles * pixelsPerTile,sideOccupiedTiles * pixelsPerTile };
	}
	break;
	case GFX::EntityTypeWrapper::EntityType::MEDIUM_ELECTRIC_POLE:
	{
		const int sideOccupiedTiles = getSideOccupiesTilesByElectricPoleType(Entities::ELECTRIC_POLE_TYPE::MEDIUM);
		return { sideOccupiedTiles * pixelsPerTile,sideOccupiedTiles * pixelsPerTile };
	}
	break;
	case GFX::EntityTypeWrapper::EntityType::BIG_ELECTRIC_POLE:
	{
		const int sideOccupiedTiles = getSideOccupiesTilesByElectricPoleType(Entities::ELECTRIC_POLE_TYPE::BIG);
		return { sideOccupiedTiles * pixelsPerTile,sideOccupiedTiles * pixelsPerTile };
	}
	case GFX::EntityTypeWrapper::EntityType::SUBSTATION:
	{
		const int sideOccupiedTiles = getSideOccupiesTilesByElectricPoleType(Entities::ELECTRIC_POLE_TYPE::SUBSTATION);
		return { sideOccupiedTiles * pixelsPerTile,sideOccupiedTiles * pixelsPerTile };
	}
	break;
	case GFX::EntityTypeWrapper::EntityType::ROBOPORT:
	{
		return { Entities::RoboportSideNumTiles * pixelsPerTile,Entities::RoboportSideNumTiles * pixelsPerTile };
	}
	break;
	case GFX::EntityTypeWrapper::EntityType::RADAR:
	{
		return { Entities::RadarSideNumTiles * pixelsPerTile,Entities::RadarSideNumTiles * pixelsPerTile };
	}
	break;
	default:
		IOUtil::Asserts::assertMessage(false, "GFX::EntitiesRepMapping::entityTypeSizeConverter - Invalid entity type returning size {0,0}");
		return { 0,0 };
		break;
	}
}

void GFX::EntitiesRepMapping::setEntityRepresentationInfo()
{
	//--------SOLAR PANEL---------------------
	{
		EntityTypeWrapper solarPanel(EntityTypeWrapper::EntityType::SOLAR_PANEL);
		EntityRepresentation& solarPanelRepresentation = EntityRepresentationByEntityType[solarPanel.getEntityType()];

		ShapeWrapper* shape1 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE);
		shape1->setShapeInfo(sf::Color::Blue, { 0,0 }, { entityTypeSizeConverter(EntityTypeWrapper::EntityType::SOLAR_PANEL) });

		float shape2Scale = 0.85;
		ShapeWrapper* shape2 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE);
		floatPair sizeShape2 = { (entityTypeSizeConverter(EntityTypeWrapper::EntityType::SOLAR_PANEL).first * shape2Scale), (entityTypeSizeConverter(EntityTypeWrapper::EntityType::SOLAR_PANEL).first * shape2Scale) };
		float posShape2 = (entityTypeSizeConverter(EntityTypeWrapper::EntityType::SOLAR_PANEL).first - (sizeShape2.first)) / 2.0;
		shape2->setShapeInfo(sf::Color::Blue, { posShape2,posShape2 }, { sizeShape2 });

		ShapeWrapper* shape3 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::CONVEX_SHAPE);
		shape3->setShapeInfo(sf::Color::Blue, { posShape2,posShape2 }, { {0,0},{ sizeShape2.first, sizeShape2.first},{sizeShape2.first,0} });

		solarPanelRepresentation.emplace_back(shape1);
		solarPanelRepresentation.emplace_back(shape2);
		solarPanelRepresentation.emplace_back(shape3);
	}

	//--------ACCUMULATOR---------------------
	{
		EntityTypeWrapper accumulator(EntityTypeWrapper::EntityType::ACCUMULATOR);
		EntityRepresentation& accumulatorRepresentation = EntityRepresentationByEntityType[accumulator.getEntityType()];

		ShapeWrapper* shape1 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE);
		shape1->setShapeInfo(sf::Color::Red, { 0,0 }, { entityTypeSizeConverter(EntityTypeWrapper::EntityType::ACCUMULATOR) });

		ShapeWrapper* shape2 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::CIRCLE_SHAPE);
		shape2->setShapeInfo(sf::Color::Red, { 0,0 }, entityTypeSizeConverter(EntityTypeWrapper::EntityType::ACCUMULATOR).first / 2.0);

		accumulatorRepresentation.emplace_back(shape1);
		accumulatorRepresentation.emplace_back(shape2);
	}

	//--------SMALL ELECTRIC POLE-------------
	//--------MEDIUM ELECTRIC POLE------------
	//--------BIG ELECTRIC POLE---------------
	//--------SUBSTATION----------------------
	//--------ROBOPORT------------------------
	//--------RADAR---------------------------
}
