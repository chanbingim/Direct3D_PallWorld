#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CLight;

class CLightManager : public CBase
{
private :
	CLightManager();
	virtual ~CLightManager() = default;

public :
	void							ADDLight(CLight* pLight);
	const CLight*					GetLight(_uInt iIndex);

private :
	list<CLight*>					m_Lights;

public :
	static		CLightManager*		Create();
	virtual		void				Free() override;


};
NS_END