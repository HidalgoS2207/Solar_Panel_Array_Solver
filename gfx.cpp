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
	for (auto& entity : entities)
	{
		EntityRepresentation& entityRepList = entitiesRepMapping.getEntityRep(entity.second);
		for (auto& entityRepElem : entityRepList)
		{
			entityRepElem.draw(renderWindowRef);
		}
	}
}

void GFX::Rendereable::insert(const EntityId entityId, const EntityTypeWrapper entityTypeWrapper)
{
	entities.insert({ entityId,entityTypeWrapper });
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

	if (this->shapeType != GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE)
	{
		sf::RectangleShape* rectShape = dynamic_cast<sf::RectangleShape*>(shapePtr);

		rectShape->setSize(sf::Vector2f({ size.first,size.second }));
		rectShape->setFillColor(shapeColor);
	}
	else
	{
		IOUtil::Asserts::assertMessage(false, "GFX::ShapeWrapper::setShapeInfo - Cannot set shape properties. Parameters doesn't match the shape type.");
	}
}

void GFX::ShapeWrapper::draw(sf::RenderWindow& renderWindowRef)
{
	switch (shapeType)
	{
	case GFX::ShapeWrapper::ShapeType::CIRCLE_SHAPE:
		renderWindowRef.draw(*(dynamic_cast<sf::CircleShape*>(shapePtr)));
		break;
	case GFX::ShapeWrapper::ShapeType::CONVEX_SHAPE:
		renderWindowRef.draw(*(dynamic_cast<sf::ConvexShape*>(shapePtr)));
		break;
	case GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE:
	default:
		renderWindowRef.draw(*(dynamic_cast<sf::RectangleShape*>(shapePtr)));
		break;
	}
}

GFX::EntityTypeWrapper::EntityTypeWrapper(EntityType entityType)
	:
	entityType(entityType)
{}

GFX::EntityTypeWrapper::EntityTypeWrapper(const EntityTypeWrapper& entityTypeWrapper)
	:
	entityType(entityTypeWrapper.entityType)
{}

GFX::EntityTypeWrapper::~EntityTypeWrapper()
{}

const GFX::EntityTypeWrapper::EntityType GFX::EntityTypeWrapper::getEntityType() const
{
	return this->entityType;
}

GFX::EntitiesRepMapping::EntitiesRepMapping()
{
	setEntityRepresentationInfo();
}

GFX::EntitiesRepMapping::~EntitiesRepMapping()
{

}

GFX::EntityRepresentation& GFX::EntitiesRepMapping::getEntityRep(const EntityTypeWrapper& entityType)
{
	return EntityRepresentationByEntityType[entityType.getEntityType()];
}

void GFX::EntitiesRepMapping::setEntityRepresentationInfo()
{
	//--------SOLAR PANEL---------------------
	EntityTypeWrapper solarPanel(EntityTypeWrapper::EntityType::SOLAR_PANEL);
	std::vector<ShapeWrapper>& solarPanelRepresentation = EntityRepresentationByEntityType[solarPanel.getEntityType()];
	ShapeWrapper shape1(GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE);
	shape1.setShapeInfo(sf::Color::Red, { 0,0 }, { 30,30 });
	solarPanelRepresentation.push_back(shape1);

	//--------ACCUMULATOR---------------------


	//--------SMALL ELECTRIC POLE-------------
	//--------MEDIUM ELECTRIC POLE------------
	//--------BIG ELECTRIC POLE---------------
	//--------SUBSTATION----------------------
	//--------ROBOPORT------------------------
	//--------RADAR---------------------------
}
