#include "LogoLevel.h"

#include "GameInstance.h"

CLogoLevel::CLogoLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID) :
	CLevel(pDevice, pContext, _iID)
{
}

HRESULT CLogoLevel::Initialize()
{
	if(FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"), ENUM_CLASS(LEVEL::LOGO), TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}

void CLogoLevel::Update(_float fTimeDelta)
{
	if (m_pGameInstance->KeyUp(KEY_INPUT::KEYBOARD, DIK_SPACE))
	{
		OutputDebugString(TEXT("keyDown SPACE"));
	}
}

HRESULT CLogoLevel::Render()
{
	SetWindowText(g_hWnd, TEXT("로고레벨이빈다"));

	return S_OK;
}

CLogoLevel* CLogoLevel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eID)
{
	CLogoLevel* pLogoLevel = new CLogoLevel(pDevice, pContext, ENUM_CLASS(eID));
	if(FAILED(pLogoLevel->Initialize()))
	{
		Safe_Release(pLogoLevel);
		MSG_BOX("CREATE FAIL : LOGO LEVEL");
	}
	return pLogoLevel;
}

void CLogoLevel::Free()
{
	__super::Free();
}
