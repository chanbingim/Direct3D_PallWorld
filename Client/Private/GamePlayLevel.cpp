#include "GamePlayLevel.h"

#include "GameInstance.h"
#include "GamePlayHUD.h"

CGamePlayLevel::CGamePlayLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID) :
	CLevel(pDevice, pContext, _iID)
{
}

HRESULT CGamePlayLevel::Initialize()
{
	if (FAILED(Setting_GamePlayHUD()))
		return E_FAIL;

	if (FAILED(ADD_CameraLayer(TEXT("Layer_GamePlay_Camera"))))
		return E_FAIL;

	if (FAILED(ADD_BackGroundLayer(TEXT("Layer_GamePlay_"))))
		return E_FAIL;

	return S_OK;
}

void CGamePlayLevel::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

HRESULT CGamePlayLevel::Render()
{
	SetWindowText(g_hWnd, TEXT("게임 플레이 레벨"));
	return S_OK;
}

HRESULT CGamePlayLevel::ADD_CameraLayer(const _wstring& LayerName)
{
	CBaseCamera::CAMERA_DESC Camera_Desc = {};

	Camera_Desc.vEye = { 0.f, 20.f, -20.f };
	Camera_Desc.vAt = { 0.f, 0.f, 0.f};

	Camera_Desc.fFov = XMConvertToRadians(60.f);
	Camera_Desc.fNear = 0.1f;
	Camera_Desc.fFar = 1000.f;

	if(FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera"),
				ENUM_CLASS(LEVEL::GAMEPLAY), LayerName, &Camera_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGamePlayLevel::ADD_BackGroundLayer(const _wstring& LayerName)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrian"),
				ENUM_CLASS(LEVEL::GAMEPLAY), LayerName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGamePlayLevel::Setting_GamePlayHUD()
{
	CGamePlayHUD* pGamePlayHUD = CGamePlayHUD::Create(m_pGraphic_Device, m_pDeviceContext);
	SetHUD(pGamePlayHUD);
	if (FAILED(pGamePlayHUD->Initialize()))
		return E_FAIL;

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
