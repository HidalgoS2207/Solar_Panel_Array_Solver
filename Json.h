#pragma once

#include "Entity.h"
#include "Util.h"

#include <string>
#include <fstream>

namespace Output
{	
	static const unsigned long long int versionNum = 281479278297089;

	class Json
	{
	private:
		const char* entityNumOcurrence;
		const char* entityNameOcurrence;
		const char* entityXPosOcurrence;
		const char* entityYPosOcurrence;
		const char* footerVersionNumberOcurrence;
	public:
		Json();
		~Json();

		void saveToFile(const char* fileName, const std::vector<Entities::Entity*>& entityList);
	private:
		void insertEntity(Entities::Entity* entity, const unsigned int entityNumber);
		void insertFooter();

		void replaceOccurrence(std::string occurrence, std::string replacement);
		
		using uintPair = std::pair<unsigned int, unsigned int >;
		std::map<uintPair, unsigned int> entityNumberByEntityPosition;

		std::string outputStr;
		std::string genericEntityTemplate;
		std::string electricPoleTemplate;
		std::string footerTemplate;
	};
}

