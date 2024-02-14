#include "Json.h"

Output::Json::Json()
{
	outputStr.assign("{\"blueprint\":{\"icons\":[{\"signal\":{\"type\":\"item\",\"name\":\"solar-panel\"},\"index\":1},{\"signal\":{\"type\":\"item\",\"name\":\"accumulator\"},\"index\":2}]}");
}

Output::Json::~Json() {}

void Output::Json::insertEntity(const Entities::Entity* const entity)
{

}

void Output::Json::saveToFile(const char* fileName)
{

}


