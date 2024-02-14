#pragma once

#include "Entity.h"

#include <string>

namespace Output
{	
	class Json
	{
	public:
		Json();
		~Json();

		void insertEntity(const Entities::Entity* const entity);
		void saveToFile(const char* fileName);
	private:
		std::string outputStr;
	};
}

