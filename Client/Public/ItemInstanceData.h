#pragma once

#include "Base.h"

NS_BEGIN(Client)
class CItemInstanceData : public CBase
{
private :
	CItemInstanceData();
	virtual ~CItemInstanceData() = default;

public :


private :








public :
	static		CItemInstanceData*	Create();
	virtual		void				Free() override;

};
NS_END