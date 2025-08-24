#pragma once
#include "Client_Define.h"
#include "Level.h"

NS_BEGIN(Client)
class CEditorCamera;

class CEditorLevel : public CLevel
{
private:
	CEditorLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID);
	virtual ~CEditorLevel() = default;

public:
	virtual HRESULT			Initialize();
	virtual void			Update(_float fTimeDelta);
	virtual HRESULT			Render();

private :
	CEditorCamera*			m_pEditorCamera = nullptr;


private :
	HRESULT					ADD_EditorCameraLayer(const WCHAR* LayerName);

public:
	static	CEditorLevel*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt iID);
	virtual void			Free() override;
};
NS_END