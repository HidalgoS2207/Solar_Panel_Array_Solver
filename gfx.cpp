#include "gfx.h"

GFX::Window::Window(const double renderDelaySeconds)
	:
	event(),
	nanosecsPerSecond(1000000000.0),
	renderDelayNanoSeconds(renderDelaySeconds * nanosecsPerSecond),
	BASE_FPS(60.0),
	FPS(60.0),
	NANOS_PER_FRAME((1000.0) / (FPS) * (1000000.0))
{
	renderWindow = new sf::RenderWindow(sf::VideoMode(1280, 960), "Enhanced Packing Algorithm Visualization");
}

GFX::Window::~Window()
{
	delete renderWindow;
}

void GFX::Window::render(const double currentDelayCountSeconds)
{
	double currentDelayNanoSeconds = renderDelayNanoSeconds;
	if (
		currentDelayCountSeconds <= 10.0 &&
		currentDelayCountSeconds > 0.0)
	{ currentDelayNanoSeconds = currentDelayCountSeconds * nanosecsPerSecond; }

	delayCount = delayCount.zero();
	t2 = std::chrono::steady_clock::now();
	do
	{
		instantTime = std::chrono::steady_clock::now();

		t_diff = instantTime - t1;
		delayCount = instantTime - t2;

		if (t_diff.count() >= NANOS_PER_FRAME)
		{
			t1 = std::chrono::steady_clock::now();

			renderWindow->clear();

			renderableObjets.draw(*renderWindow);

			renderWindow->display();

			handleEvents();
		}

	} while (delayCount.count() < currentDelayNanoSeconds); 	//Keep rendering until the delay is complete then continue with the calculations
}

void GFX::Window::updateRendereablePosition(const EntityId id, const floatPair pos)
{
	renderableObjets.updateEntityPosition(id, pos);
}

void GFX::Window::resetRendereablePosition(const EntityId id)
{
	renderableObjets.resetEntityPosition(id);
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
		if (!entity.second.getIsVisible()) { continue; } //Early for loop control return because we don't want to render an invisible shape
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

void GFX::Rendereable::updateEntityPosition(const EntityId entityId, const floatPair pos, const bool isVisible)
{
	EntityTypeWrapperByEntityId::iterator entityTypeWrapperIt = entityTypeWrapperByEntityId.find(entityId);

	if (entityTypeWrapperIt == entityTypeWrapperByEntityId.end())
	{
		IOUtil::Asserts::assertMessage(false, "GFX::Rendereable::updateEntityPosition - Position cannot be updated entityId couldn't be found.");
		return;
	}

	entityTypeWrapperIt->second.setPosition(entitiesRepMapping.entityPositionToTilePosition(pos), isVisible);
}

void GFX::Rendereable::resetEntityPosition(const EntityId entityId)
{
	updateEntityPosition(entityId, { 0.0,0.0 }, false);
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
		case Entities::ELECTRIC_POLE_TYPE::SMALL:
			return EntityTypeWrapper(GFX::EntityTypeWrapper::EntityType::SMALL_ELECTRIC_POLE);
			break;
		case Entities::ELECTRIC_POLE_TYPE::MEDIUM:
			return EntityTypeWrapper(GFX::EntityTypeWrapper::EntityType::MEDIUM_ELECTRIC_POLE);
			break;
		case Entities::ELECTRIC_POLE_TYPE::BIG:
			return EntityTypeWrapper(GFX::EntityTypeWrapper::EntityType::BIG_ELECTRIC_POLE);
			break;
		case Entities::ELECTRIC_POLE_TYPE::SUBSTATION:
			return EntityTypeWrapper(GFX::EntityTypeWrapper::EntityType::SUBSTATION);
			break;
		case Entities::ELECTRIC_POLE_TYPE::INVALID:
		default:
			IOUtil::Asserts::assertMessage(!GFX::verboseExecution, "GFX::EntityTypeWrapper GFX::Rendereable::getEntityTypeWrapper - Invalid Electric Pole Type.");
			return EntityTypeWrapper(GFX::EntityTypeWrapper::EntityType::INVALID);
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
	case Entities::ENTITY_TYPE::INVALID:
	default:
		IOUtil::Asserts::assertMessage(false, "GFX::EntityTypeWrapper GFX::Rendereable::getEntityTypeWrapper - Invalid Entity Type.");
		return EntityTypeWrapper(GFX::EntityTypeWrapper::EntityType::INVALID);
		break;
	}
}

const char* GFX::ShapeWrapper::sFontFilePath = "Assets/PressStart2P-Regular.ttf";
const unsigned int GFX::ShapeWrapper::sInitialFontSize = 10;

GFX::ShapeWrapper::ShapeWrapper(ShapeType shapeType)
	:
	shapeType(shapeType),
	shapePtr(nullptr),
	textPtr(nullptr)
{
	relPos = { 0.0,0.0 };

	switch (shapeType)
	{
	case GFX::ShapeWrapper::ShapeType::TEXT:
	{
		if (font.loadFromFile(sFontFilePath))
		{
			textPtr = new sf::Text;
			textPtr->setFont(font);
			textPtr->setCharacterSize(sInitialFontSize);
		}
		else
		{
			IOUtil::Asserts::assertMessage(false, "GFX::ShapeWrapper::ShapeWrapper - Cannot initialize textPtr. Invalid font file path or file name.");
		}
	}
	break;
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
	shapeType(shapeWrapperRef.shapeType),
	shapePtr(nullptr),
	textPtr(nullptr)
{
	if (shapeWrapperRef.shapePtr != nullptr)
	{
		this->shapePtr = shapeWrapperRef.shapePtr;
	}

	if (shapeWrapperRef.textPtr != nullptr)
	{
		this->textPtr = shapeWrapperRef.textPtr;
	}

	this->relPos = shapeWrapperRef.relPos;
}

GFX::ShapeWrapper::~ShapeWrapper()
{
	delete shapePtr;
	delete textPtr;
}

void GFX::ShapeWrapper::setShapeInfo(const sf::Color shapeColor, const floatPair relPos, const floatPair size)
{
	this->relPos = relPos;

	if (this->shapeType == GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE)
	{
		sf::RectangleShape* rectShape = dynamic_cast<sf::RectangleShape*>(shapePtr);

		rectShape->setSize(sf::Vector2f({ size.first,size.second }));
		rectShape->setFillColor(sf::Color::Transparent);
		rectShape->setOutlineColor(shapeColor);
		rectShape->setOutlineThickness(-1.0);
		rectShape->setScale({ 1.0,1.0 });
	}
	else
	{
		IOUtil::Asserts::assertMessage(false, "GFX::ShapeWrapper::setShapeInfo - Cannot set shape properties. Parameters doesn't match the shape type. Expected RECTANGLE_SHAPE.");
	}
}

void GFX::ShapeWrapper::setShapeInfo(const sf::Color shapeColor, const floatPair relPos, const float radius)
{
	this->relPos = relPos;

	if (this->shapeType == GFX::ShapeWrapper::ShapeType::CIRCLE_SHAPE)
	{
		sf::CircleShape* circleShape = dynamic_cast<sf::CircleShape*>(shapePtr);

		circleShape->setRadius(radius);
		circleShape->setFillColor(sf::Color::Transparent);
		circleShape->setOutlineColor(shapeColor);
		circleShape->setOutlineThickness(-1.0);
		circleShape->setScale({ 1.0,1.0 });
	}
	else
	{
		IOUtil::Asserts::assertMessage(false, "GFX::ShapeWrapper::setShapeInfo - Cannot set shape properties. Parameters doesn't match the shape type. Expected CIRCLE_SHAPE.");
	}
}

void GFX::ShapeWrapper::setShapeInfo(const sf::Color shapeColor, const floatPair relPos, const PointsList pointsList)
{
	this->relPos = relPos;

	if (this->shapeType == GFX::ShapeWrapper::ShapeType::CONVEX_SHAPE)
	{
		sf::ConvexShape* convexShape = dynamic_cast<sf::ConvexShape*>(shapePtr);
		convexShape->setFillColor(sf::Color::Transparent);
		convexShape->setOutlineColor(shapeColor);
		convexShape->setOutlineThickness(-1.0);
		convexShape->setScale({ 1.0,1.0 });

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
		IOUtil::Asserts::assertMessage(false, "GFX::ShapeWrapper::setShapeInfo - Cannot set shape properties. Parameters doesn't match the shape type. Expected CONVEX_SHAPE.");
	}
}

void GFX::ShapeWrapper::setShapeInfo(const sf::Color shapeColor, const floatPair relPos, const char* textToDisplay, const unsigned int fontSize)
{
	this->relPos = relPos;

	if (this->shapeType == GFX::ShapeWrapper::ShapeType::TEXT)
	{
		textPtr->setString(textToDisplay);
		textPtr->setFillColor(shapeColor);
		textPtr->setCharacterSize(fontSize);
		textPtr->setScale({ 1.0,1.0 });
	}
	else
	{
		IOUtil::Asserts::assertMessage(false, "GFX::ShapeWrapper::setShapeInfo - Cannot set shape properties. Parameters doesn't match the shape type. Expected TEXT.");
	}
}

void GFX::ShapeWrapper::setPosition(const floatPair absPosition)
{
	IOUtil::Asserts::assertMessage((shapePtr != nullptr) || (textPtr != nullptr), "GFX::ShapeWrapper::setPosition - TextPtr and ShapePtr not initialized.");

	if (shapePtr != nullptr) { shapePtr->setPosition({ absPosition.first + relPos.first,absPosition.second + relPos.second }); }
	if (textPtr != nullptr) { textPtr->setPosition({ absPosition.first + relPos.first,absPosition.second + relPos.second }); }
}

void GFX::ShapeWrapper::resetPosition()
{
	this->setPosition({ 0.0,0.0 });
}

void GFX::ShapeWrapper::draw(sf::RenderWindow& renderWindowRef)
{
	IOUtil::Asserts::assertMessage((shapePtr != nullptr) || (textPtr != nullptr), "GFX::ShapeWrapper::draw - TextPtr and ShapePtr not initialized.");

	if (shapePtr != nullptr) { renderWindowRef.draw(*shapePtr); }
	if (textPtr != nullptr) { renderWindowRef.draw(*textPtr); }
}

GFX::EntityTypeWrapper::EntityTypeWrapper(EntityType entityType)
	:
	entityType(entityType),
	position(0.0, 0.0),
	isVisible(false)
{}

GFX::EntityTypeWrapper::EntityTypeWrapper(const EntityTypeWrapper& entityTypeWrapper)
	:
	entityType(entityTypeWrapper.entityType),
	position(entityTypeWrapper.position),
	isVisible(entityTypeWrapper.isVisible)
{}

GFX::EntityTypeWrapper::~EntityTypeWrapper()
{}

const GFX::floatPair& GFX::EntityTypeWrapper::getPosition()
{
	return position;
}

void GFX::EntityTypeWrapper::setPosition(floatPair position, const bool isVisible)
{
	this->isVisible = isVisible;
	this->position = position;
}

void GFX::EntityTypeWrapper::resetPosition()
{
	setPosition({ 0.0,0.0 }, false);
}

const GFX::EntityTypeWrapper::EntityType GFX::EntityTypeWrapper::getEntityType() const
{
	return this->entityType;
}

const bool GFX::EntityTypeWrapper::getIsVisible() const
{
	return isVisible;
}

const float GFX::EntitiesRepMapping::sPixelsPerTile = 20.0;

GFX::EntitiesRepMapping::EntitiesRepMapping()
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
	GFX::EntityRepresentationByEntityType::iterator EntityRepresentationByEntityTypeIt = EntityRepresentationByEntityType.find(entityType.getEntityType());
	IOUtil::Asserts::assertMessage(EntityRepresentationByEntityTypeIt != EntityRepresentationByEntityType.end(), "GFX::EntitiesRepMapping::getEntityRep - Entity representation cannot be identified.");
	return EntityRepresentationByEntityTypeIt->second;
}

GFX::floatPair GFX::EntitiesRepMapping::entityTypeSizeConverter(const GFX::EntityTypeWrapper::EntityType entityType)
{
	auto getSideOccupiesTilesByElectricPoleType = [](const Entities::ELECTRIC_POLE_TYPE electricPoleType) -> const float
		{
			return static_cast<float>(std::sqrt(Entities::ElectricPoleAreaOccupiedByType::ElectricPoleAreaOccupied.at(electricPoleType)));
		};

	switch (entityType)
	{
	case GFX::EntityTypeWrapper::EntityType::SOLAR_PANEL:
		return { static_cast<float>(Entities::SolarPanelSideNumTiles) * sPixelsPerTile,static_cast<float>(Entities::SolarPanelSideNumTiles) * sPixelsPerTile };
		break;
	case GFX::EntityTypeWrapper::EntityType::ACCUMULATOR:
		return { static_cast<float>(Entities::AccumulatorSideNumTiles) * sPixelsPerTile,static_cast<float>(Entities::AccumulatorSideNumTiles) * sPixelsPerTile };
		break;
	case GFX::EntityTypeWrapper::EntityType::SMALL_ELECTRIC_POLE:
	{
		const float sideOccupiedTiles = getSideOccupiesTilesByElectricPoleType(Entities::ELECTRIC_POLE_TYPE::SMALL);
		return { sideOccupiedTiles * sPixelsPerTile,sideOccupiedTiles * sPixelsPerTile };
	}
	break;
	case GFX::EntityTypeWrapper::EntityType::MEDIUM_ELECTRIC_POLE:
	{
		const float sideOccupiedTiles = getSideOccupiesTilesByElectricPoleType(Entities::ELECTRIC_POLE_TYPE::MEDIUM);
		return { sideOccupiedTiles * sPixelsPerTile,sideOccupiedTiles * sPixelsPerTile };
	}
	break;
	case GFX::EntityTypeWrapper::EntityType::BIG_ELECTRIC_POLE:
	{
		const float sideOccupiedTiles = getSideOccupiesTilesByElectricPoleType(Entities::ELECTRIC_POLE_TYPE::BIG);
		return { sideOccupiedTiles * sPixelsPerTile,sideOccupiedTiles * sPixelsPerTile };
	}
	case GFX::EntityTypeWrapper::EntityType::SUBSTATION:
	{
		const float sideOccupiedTiles = getSideOccupiesTilesByElectricPoleType(Entities::ELECTRIC_POLE_TYPE::SUBSTATION);
		return { sideOccupiedTiles * sPixelsPerTile,sideOccupiedTiles * sPixelsPerTile };
	}
	break;
	case GFX::EntityTypeWrapper::EntityType::ROBOPORT:
	{
		return { static_cast<float>(Entities::RoboportSideNumTiles) * sPixelsPerTile,static_cast<float>(Entities::RoboportSideNumTiles) * sPixelsPerTile };
	}
	break;
	case GFX::EntityTypeWrapper::EntityType::RADAR:
	{
		return { static_cast<float>(Entities::RadarSideNumTiles) * sPixelsPerTile,static_cast<float>(Entities::RadarSideNumTiles) * sPixelsPerTile };
	}
	break;
	default:
		IOUtil::Asserts::assertMessage(false, "GFX::EntitiesRepMapping::entityTypeSizeConverter - Invalid entity type returning size {0,0}");
		return { 0,0 };
		break;
	}
}

GFX::floatPair GFX::EntitiesRepMapping::entityPositionToTilePosition(const floatPair position)
{
	IOUtil::Asserts::assertMessageFormatted(!GFX::verboseExecution, "PosX = [%f] -> [%f] | PosY = [%f] -> [%f]", position.first, position.first * sPixelsPerTile, position.second, position.second * sPixelsPerTile);
	return { position.first * sPixelsPerTile,position.second * sPixelsPerTile };
}

void GFX::EntitiesRepMapping::setEntityRepresentationInfo()
{
	//--------SOLAR PANEL---------------------
	{
		EntityTypeWrapper solarPanel(EntityTypeWrapper::EntityType::SOLAR_PANEL);
		EntityRepresentation& solarPanelRepresentation = EntityRepresentationByEntityType[solarPanel.getEntityType()];

		ShapeWrapper* shape1 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE);
		shape1->setShapeInfo(sf::Color::Blue, { 0.0,0.0 }, { entityTypeSizeConverter(EntityTypeWrapper::EntityType::SOLAR_PANEL) });

		const float shape2Scale = 0.85;
		ShapeWrapper* shape2 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE);
		floatPair sizeShape2 = { (entityTypeSizeConverter(EntityTypeWrapper::EntityType::SOLAR_PANEL).first * shape2Scale), (entityTypeSizeConverter(EntityTypeWrapper::EntityType::SOLAR_PANEL).first * shape2Scale) };
		const float posShape2 = (entityTypeSizeConverter(EntityTypeWrapper::EntityType::SOLAR_PANEL).first - (sizeShape2.first)) / 2.0;
		shape2->setShapeInfo(sf::Color::Blue, { posShape2,posShape2 }, { sizeShape2 });

		ShapeWrapper* shape3 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::TEXT);
		shape3->setShapeInfo(sf::Color::White, { posShape2 + 3,posShape2 + 3 }, "SOLAR\nPANEL", 10);

		solarPanelRepresentation.emplace_back(shape1);
		solarPanelRepresentation.emplace_back(shape2);
		solarPanelRepresentation.emplace_back(shape3);
	}

	//--------ACCUMULATOR---------------------
	{
		EntityTypeWrapper accumulator(EntityTypeWrapper::EntityType::ACCUMULATOR);
		EntityRepresentation& accumulatorRepresentation = EntityRepresentationByEntityType[accumulator.getEntityType()];

		ShapeWrapper* shape1 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE);
		shape1->setShapeInfo(sf::Color::Red, { 0.0,0.0 }, { entityTypeSizeConverter(EntityTypeWrapper::EntityType::ACCUMULATOR) });

		ShapeWrapper* shape2 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::CIRCLE_SHAPE);
		shape2->setShapeInfo(sf::Color::Red, { 0.0,0.0 }, entityTypeSizeConverter(EntityTypeWrapper::EntityType::ACCUMULATOR).first / 2.0);

		ShapeWrapper* shape3 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::TEXT);
		shape3->setShapeInfo(sf::Color::White, { 4.0,15.0 }, "ACC");

		accumulatorRepresentation.emplace_back(shape1);
		accumulatorRepresentation.emplace_back(shape2);
		accumulatorRepresentation.emplace_back(shape3);
	}

	//--------SMALL ELECTRIC POLE-------------
	{
		EntityTypeWrapper mediumElectricPole(EntityTypeWrapper::EntityType::SMALL_ELECTRIC_POLE);
		EntityRepresentation& mediumElectricPoleRepresentation = EntityRepresentationByEntityType[mediumElectricPole.getEntityType()];

		ShapeWrapper* shape1 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE);
		shape1->setShapeInfo(sf::Color::White, { 0,0 }, { entityTypeSizeConverter(EntityTypeWrapper::EntityType::SMALL_ELECTRIC_POLE) });

		ShapeWrapper* shape2 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::TEXT);
		shape2->setShapeInfo(sf::Color::White, { 2,5 }, "SP", 9);

		mediumElectricPoleRepresentation.emplace_back(shape1);
		mediumElectricPoleRepresentation.emplace_back(shape2);
	}

	//--------MEDIUM ELECTRIC POLE------------
	{
		EntityTypeWrapper mediumElectricPole(EntityTypeWrapper::EntityType::MEDIUM_ELECTRIC_POLE);
		EntityRepresentation& mediumElectricPoleRepresentation = EntityRepresentationByEntityType[mediumElectricPole.getEntityType()];

		ShapeWrapper* shape1 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE);
		shape1->setShapeInfo(sf::Color::White, { 0,0 }, { entityTypeSizeConverter(EntityTypeWrapper::EntityType::MEDIUM_ELECTRIC_POLE) });

		ShapeWrapper* shape2 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::TEXT);
		shape2->setShapeInfo(sf::Color::White, { 2,5 }, "MP", 9);

		mediumElectricPoleRepresentation.emplace_back(shape1);
		mediumElectricPoleRepresentation.emplace_back(shape2);
	}

	//--------BIG ELECTRIC POLE---------------
	{
		EntityTypeWrapper bigElectricPole(EntityTypeWrapper::EntityType::BIG_ELECTRIC_POLE);
		EntityRepresentation& bigElectricPoleRepresentation = EntityRepresentationByEntityType[bigElectricPole.getEntityType()];

		ShapeWrapper* shape1 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE);
		shape1->setShapeInfo(sf::Color::White, { 0,0 }, { entityTypeSizeConverter(EntityTypeWrapper::EntityType::BIG_ELECTRIC_POLE) });

		ShapeWrapper* shape2 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::TEXT);
		shape2->setShapeInfo(sf::Color::White, { 5,5 }, "BP");

		bigElectricPoleRepresentation.emplace_back(shape1);
		bigElectricPoleRepresentation.emplace_back(shape2);
	}

	//--------SUBSTATION----------------------
	{
		EntityTypeWrapper substation(EntityTypeWrapper::EntityType::SUBSTATION);
		EntityRepresentation& substationRepresentation = EntityRepresentationByEntityType[substation.getEntityType()];

		ShapeWrapper* shape1 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE);
		shape1->setShapeInfo(sf::Color::White, { 0,0 }, { entityTypeSizeConverter(EntityTypeWrapper::EntityType::SUBSTATION) });

		ShapeWrapper* shape2 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::TEXT);
		shape2->setShapeInfo(sf::Color::White, { 5,5 }, "SUB");

		substationRepresentation.emplace_back(shape1);
		substationRepresentation.emplace_back(shape2);
	}

	//--------ROBOPORT------------------------
	{
		EntityTypeWrapper roboport(EntityTypeWrapper::EntityType::ROBOPORT);
		EntityRepresentation& roboportRepresentation = EntityRepresentationByEntityType[roboport.getEntityType()];

		ShapeWrapper* shape1 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE);
		shape1->setShapeInfo(sf::Color::White, { 0,0 }, { entityTypeSizeConverter(EntityTypeWrapper::EntityType::ROBOPORT) });

		ShapeWrapper* shape2 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::TEXT);
		shape2->setShapeInfo(sf::Color::White, { 2,5 }, "ROB");

		roboportRepresentation.emplace_back(shape1);
		roboportRepresentation.emplace_back(shape2);
	}

	//--------RADAR---------------------------
	{
		EntityTypeWrapper radar(EntityTypeWrapper::EntityType::RADAR);
		EntityRepresentation& radarRepresentation = EntityRepresentationByEntityType[radar.getEntityType()];

		ShapeWrapper* shape1 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::RECTANGLE_SHAPE);
		shape1->setShapeInfo(sf::Color::White, { 0,0 }, { entityTypeSizeConverter(EntityTypeWrapper::EntityType::RADAR) });

		ShapeWrapper* shape2 = new ShapeWrapper(GFX::ShapeWrapper::ShapeType::TEXT);
		shape2->setShapeInfo(sf::Color::White, { 2,5 }, "RAD");

		radarRepresentation.emplace_back(shape1);
		radarRepresentation.emplace_back(shape2);
	}
}
