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
	void							RemoveLight(CLight* pLight);
	const CLight*					GetLight(_uInt iIndex);

	HRESULT							Render_Lights(class CShader* pShader, class CVIBuffer* pVIBuffer);

private :
	list<CLight*>					m_Lights;

public :
	static		CLightManager*		Create();
	virtual		void				Free() override;


};
NS_END