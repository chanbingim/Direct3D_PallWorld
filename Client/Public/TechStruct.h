#pragma once

#include "Engine_Defines.h"

namespace Client
{
	enum class TECH_TYPE {  ARCHITECTURE, PELL, ITEM, END };

	typedef struct TechItemDesc
	{
		_uInt				TechTypeID;
		TECH_TYPE			TechType;

		vector<_uInt>		IngredientItemIDs;
		vector<_uInt>		IngredientItemCounts;
		
		_uInt				NecessaryLevel;
		_uInt				NecessaryPoint;

		_uInt				ReturnItemID;
		_bool				LearnTech;
	}TECH_ITEM_DESC;
}