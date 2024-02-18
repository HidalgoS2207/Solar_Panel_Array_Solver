#include "Json.h"

Output::Json::Json()
	:
	entityNumOcurrence("@ENTITY_NUM"),
	entityNameOcurrence("@ENTITY_NAME"),
	entityXPosOcurrence("@ENTITY_X_POS"),
	entityYPosOcurrence("@ENTITY_Y_POS"),
	footerVersionNumberOcurrence("@FOOTER_VERSION_NUMBER")
{
	outputStr.assign("{\"blueprint\":{\"icons\":[{\"signal\":{\"type\":\"item\",\"name\":\"solar-panel\"},\"index\":1},{\"signal\":{\"type\":\"item\",\"name\":\"accumulator\"},\"index\":2}],\"entities\":[],}}");

	genericEntityTemplate.assign("{\"entity_number\":@ENTITY_NUM,\"name\":\"@ENTITY_NAME\",\"position\":{\"x\":@ENTITY_X_POS,\"y\":@ENTITY_Y_POS}},");
	electricPoleTemplate.assign("{\"entity_number\":@ENTITY_NUM,\"name\":\"@ENTITY_NAME\",\"position\":{\"x\":@ENTITY_X_POS,\"y\":@ENTITY_Y_POS},\"neighbours\":[]},");
	footerTemplate.assign("\"item\":\"blueprint\",\"version\":@FOOTER_VERSION_NUMBER");
}

Output::Json::~Json() {}

void Output::Json::insertEntity(Entities::Entity* entity, const unsigned int entityNumber)
{
	std::string entiyNumberStr = std::to_string(entityNumber);
	std::string entityNameStr = entity->getEntityName();
	std::string entityXposStr = std::to_string(static_cast<float>(entity->getPosition().first) + (static_cast<float>(entity->getTilesDistribution().first) / 2.f));
	std::string entityYposStr = std::to_string(static_cast<float>(entity->getPosition().second) + (static_cast<float>(entity->getTilesDistribution().second) / 2.f));

	std::string::iterator insPos = outputStr.end() - 4;

	auto replaceGenericOcurrences = [&]()
		{
			replaceOccurrence(entityNumOcurrence, entiyNumberStr);
			replaceOccurrence(entityNameOcurrence, entityNameStr);
			replaceOccurrence(entityXPosOcurrence, entityXposStr);
			replaceOccurrence(entityYPosOcurrence, entityYposStr);
		};

	switch (entity->getEntityType())
	{
	case Entities::ENTITY_TYPE::SOLAR_PANEL:
	case Entities::ENTITY_TYPE::ACCUMULATOR:
		outputStr.insert(insPos, genericEntityTemplate.begin(), genericEntityTemplate.end());
		replaceGenericOcurrences();
		break;
	case Entities::ENTITY_TYPE::ELECTRIC_POLE:
		outputStr.insert(insPos, electricPoleTemplate.begin(), electricPoleTemplate.end());
		replaceGenericOcurrences();
		//Neighbours insertion
		{
			Entities::ElectricPole* electricPolePtr = dynamic_cast<Entities::ElectricPole*>(entity);

			insPos = outputStr.end() - 7;

			for (int i = 0; i < electricPolePtr->getNeighbours().size(); i++)
			{
				uintPair neighbourPos = electricPolePtr->getNeighbours()[i]->getPosition();

				std::string entiyNumberStr = std::to_string(entityNumberByEntityPosition[neighbourPos]);
				if ((i + 1) != electricPolePtr->getNeighbours().size())
				{
					entiyNumberStr.append(",");
				}
				outputStr.insert(insPos, entiyNumberStr.begin(), entiyNumberStr.end());
				insPos += entiyNumberStr.size();
			}
		}
		break;
	default:
		break;
	}
}

void Output::Json::replaceOccurrence(std::string occurrence, std::string replacement)
{
	size_t pos = 0;
	pos = outputStr.find(occurrence.c_str());
	if (pos != outputStr.npos)
	{
		outputStr.erase(pos, occurrence.size());
		outputStr.insert(pos, replacement);
	}
}

void Output::Json::insertFooter()
{
	//remove invalid comma
	{
		size_t lastCommaPos = outputStr.find_last_of(",");
		size_t commaPos = outputStr.find_last_of(",", lastCommaPos - 1);

		outputStr.erase(commaPos, 1);
	}

	std::string::iterator insPos = outputStr.end() - 2;
	std::string footerVersionNumber = std::to_string(versionNum);

	outputStr.insert(insPos, footerTemplate.begin(), footerTemplate.end());

	replaceOccurrence(footerVersionNumberOcurrence, footerVersionNumber);
}

void Output::Json::saveToFile(const char* fileName, const std::vector<Entities::Entity*>& entityList)
{
	//Starts in position 1
	unsigned int entityNumber = 1;

	//Body
	for (Entities::Entity* entity : entityList)
	{
		entityNumberByEntityPosition[entity->getPosition()] = entityNumber;
		entityNumber++;
	}

	for (Entities::Entity* entity : entityList)
	{
		insertEntity(entity, entityNumberByEntityPosition[entity->getPosition()]);
	}

	//Footer
	insertFooter();

	//Save Output
	std::fstream file(fileName, std::ios_base::out);
	if (!file.is_open())
	{
		IOUtil::Asserts::assertMessageFormatted("Output::Json::saveToFile - Cannot open file %s", fileName);
		return;
	}
	file.write(outputStr.c_str(), outputStr.size());

	file.close();
}


