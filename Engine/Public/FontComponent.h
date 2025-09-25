#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CTransform;

class ENGINE_DLL CFontComponent : public CComponent
{
public :
	typedef struct FontDesc
	{
		const _float2*		pPoint;
		const WCHAR*		szUseFontName;
		_float2				vFontSize;
	}FONT_DESC;

private:
	CFontComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CFontComponent() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;

	HRESULT Render(const _tchar* pText, _vector vColor, _float2 Offset = {0.f, 0.f});

private:
	const _float2*					m_pPoint = nullptr;
	_wstring						m_szUseFontTag = {};
	_float2							m_vFontSize = {};

public:
	static	CFontComponent*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CComponent*				Clone(void* pArg);
	virtual void					Free() override;
	
};
NS_END