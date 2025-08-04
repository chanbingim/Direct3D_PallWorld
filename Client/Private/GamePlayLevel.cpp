#include "GamePlayLevel.h"

CGamePlayLevel::CGamePlayLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID) :
	CLevel(pDevice, pContext, _iID)
{
}

HRESULT CGamePlayLevel::Initialize()
{
	return S_OK;
}

void CGamePlayLevel::Update(_float fTimeDelta)
{
}

HRESULT CGamePlayLevel::Render()
{
	SetWindowText(g_hWnd, TEXT("게임 플레이 레벨"));
	return S_OK;
}

CGamePlayLevel* CGamePlayLevel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eID)
{
	CGamePlayLevel* pGamePlayLevel = new CGamePlayLevel(pDevice, pContext, ENUM_CLASS(eID));
	if (nullptr == pGamePlayLevel)
	{
		Safe_Release(pGamePlayLevel);
		MSG_BOX("CREATE FAIL : GAME PLAY LEVEL");
	}
	return pGamePlayLevel;
}

void CGamePlayLevel::Free()
{
	__super::Free();
}
