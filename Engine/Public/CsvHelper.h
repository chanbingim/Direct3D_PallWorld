#pragma once
#include "Base.h"

NS_BEGIN(Engine)

template <typename STR_t>
void	CSV_Read(const STR_t* pfilePath, vector<_string>& pOut)
{
	ifstream fFileStream(pfilePath);
	string Row, Col, Cell;
	if (!fFileStream.is_open())
		return;

	while (getline(fFileStream, Col))
	{
		stringstream sStream(Col);
		while (getline(sStream, Row, ','))
		{
			if ('"' == Row.front())
				Row.erase(Row.begin());
			if ('"' == Row.back())
				Row.pop_back();

			pOut.push_back(Row);
		}
	}

	fFileStream.close();
}

NS_END