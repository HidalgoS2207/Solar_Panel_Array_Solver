#pragma once

#include "Entity.h"
#include "Util.h"

#include <string>
#include <fstream>

namespace Output
{	
	static const unsigned int versionNum = 281479278297089;

	class Json
	{
	private:
		const char* entityNumOcurrence;
		const char* entityXPosOcurrence;
		const char* entityYPosOcurrence;
	public:
		Json();
		~Json();

		void saveToFile(const char* fileName, const std::vector<Entities::Entity*>& entityList);
	private:
		void insertEntity(const Entities::Entity* const entity, const unsigned int entityNumber);
		std::string outputStr;

		std::string genericEntityTemplate;
		std::string electricPoleTemplate;
	};
}

