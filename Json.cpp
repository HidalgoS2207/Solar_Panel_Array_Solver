#include "Json.h"

Output::Json::Json()
	:
	entityNumOcurrence("@ENTITY_NUM"),
	entityNameOcurrence("@ENTITY_NAME"),
	entityXPosOcurrence("@ENTITY_X_POS"),
	entityYPosOcurrence("@ENTITY_Y_POS")
{
	outputStr.assign("{\"blueprint\":{\"icons\":[{\"signal\":{\"type\":\"item\",\"name\":\"solar-panel\"},\"index\":1},{\"signal\":{\"type\":\"item\",\"name\":\"accumulator\"},\"index\":2}],}");

	genericEntityTemplate.assign("{	\"entity_number\":@ENTITY_NUM,\"name\":\"@ENTITY_NAME\",\"position\":{\"x\":@ENTITY_X_POS,\"y\":@ENTITY_Y_POS}},");
	electricPoleTemplate.assign("{\"entity_number\":@ENTITY_NUM,\"name\":\"@ENTITY_NAME\",\"position\":{\"x\":@ENTITY_X_POS,\"y\":@ENTITY_Y_POS}},\"neighbours\":[]},");
}

Output::Json::~Json() {}

void Output::Json::insertEntity(const Entities::Entity* const entity, const unsigned int entityNumber)
{
	entityNumberByEntityPtr[entity] = entityNumber;

	std::string entiyNumberStr = std::to_string(entityNumber);
	std::string entityNameStr = entity->getEntityName();
	std::string entityXposStr = std::to_string(static_cast<float>(entity->getPosition().first) + (static_cast<float>(entity->getTilesDistribution().first) / 2.f));
	std::string entityYposStr = std::to_string(static_cast<float>(entity->getPosition().second) + (static_cast<float>(entity->getTilesDistribution().second) / 2.f));

	std::string::iterator insPos = outputStr.end() - 2;

	auto replaceOccurrence = [&](std::string occurrence, std::string replacement)
		{
			size_t pos = 0;
			pos = outputStr.find(occurrence.c_str());
			if (pos != outputStr.npos)
			{
				outputStr.erase(pos, occurrence.size());
				outputStr.insert(pos, replacement);
			}
		};

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
		outputStr.insert(insPos, genericEntityTemplate.begin(), genericEntityTemplate.end() - 1);
		replaceGenericOcurrences();
		break;
	case Entities::ENTITY_TYPE::ELECTRIC_POLE:
		outputStr.insert(insPos, electricPoleTemplate.begin(), electricPoleTemplate.end() - 1);
		replaceGenericOcurrences();
		break;
	default:
		break;
	}
}

void Output::Json::saveToFile(const char* fileName, const std::vector<Entities::Entity*>& entityList)
{
	unsigned int entityNumber = 0;

	//Body
	for (Entities::Entity* entity : entityList)
	{
		insertEntity(entity, entityNumber);
		entityNumber++;
	}

	//Footer


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


