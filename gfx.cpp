#include "gfx.h"

GFX::Window::Window()
{
	renderWindow = new sf::RenderWindow(sf::VideoMode(800,600), "Enhanced Packing Algorithm Visualization");
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

void GFX::Window::declareRendereable(EntityId entityId, Entities::ENTITY_TYPE entityType, Entities::ELECTRIC_POLE_TYPE electricPoleType)
{
	
}

GFX::Rendereable::Rendereable(EntitiesRepMapping& entitiesRepMapping)
	:
	entitiesRepMapping(entitiesRepMapping),
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

GFX::ShapeWrapper::~ShapeWrapper()
{
	delete shapePtr;
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

GFX::EntityTypeWrapper::~EntityTypeWrapper()
{}

GFX::EntitiesRepMapping::EntitiesRepMapping()
{

}

GFX::EntitiesRepMapping::~EntitiesRepMapping()
{

}

GFX::EntityRepresentation& GFX::EntitiesRepMapping::getEntityRep(const EntityTypeWrapper& entityType)
{
	return EntityRepresentationByEntityType[entityType];
}

void GFX::EntitiesRepMapping::setEntityRepresentationInfo()
{
	//--------SOLAR PANEL---------------------
	EntityTypeWrapper solarPanel(EntityTypeWrapper::EntityType::SOLAR_PANEL);
	std::vector<ShapeWrapper>& solarPanelRepresentation = EntityRepresentationByEntityType[solarPanel];
	ShapeWrapper shape1(GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE);
	
	solarPanelRepresentation.push_back(shape1);
	
	//--------ACCUMULATOR---------------------
	 
	
	//--------SMALL ELECTRIC POLE-------------
	//--------MEDIUM ELECTRIC POLE------------
	//--------BIG ELECTRIC POLE---------------
	//--------SUBSTATION----------------------
	//--------ROBOPORT------------------------
	//--------RADAR---------------------------
}
