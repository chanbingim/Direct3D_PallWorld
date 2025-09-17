#pragma once

#include "Client_Define.h"
#include "Level.h"

NS_BEGIN(Client)
class CGamePlayLevel final : public CLevel
{
private:
	CGamePlayLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID);
	virtual ~CGamePlayLevel() = default;

public:
	virtual HRESULT			Initialize();
	virtual void			Update(_float fTimeDelta);
	virtual HRESULT			Render();

private :
	/* ADD Default Object */
	HRESULT					ADD_CameraLayer(const _wstring& LayerName);
	
	// 지형에 대한 Layer
	HRESULT					ADD_TerrianLayer(const _wstring& LayerName);
	HRESULT					ADD_SkyLayer(const _wstring& LayerName);

	// 오브젝트 및 환경 오브젝트
	HRESULT					ADD_EnviornmentLayer(const _wstring& LayerName);
	HRESULT					ADD_PlayerLayer(const _wstring& LayerName);
	HRESULT					ADD_PellLayer(const _wstring& LayerName);

	HRESULT					Setting_GamePlayHUD();

public:
	static	CGamePlayLevel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eID);
	virtual void			Free() override;
};
NS_END