#include "LogoLevel.h"

#include "GameInstance.h"
#include "LoadingLevel.h"
#include "Logo_HUD.h"

CLogoLevel::CLogoLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID) :
	CLevel(pDevice, pContext, _iID)
{
}

HRESULT CLogoLevel::Initialize()
{
	auto LogoHUD = CLogo_HUD::Create(m_pGraphic_Device, m_pDeviceContext);
	SetHUD(LogoHUD);
	if (FAILED(LogoHUD->Initialize()))
		return E_FAIL;

	m_pGameInstance->Manager_PlayBGM(TEXT("LogoSound.mp3"), 0.5f);
	return S_OK;
}

void CLogoLevel::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (m_IsChangeLevel)
	{
		auto LoadingLevel = CLoadingLevel::Create(m_pGraphic_Device, m_pDeviceContext, LEVEL::LOADING, LEVEL::GAMEPLAY);
		if (FAILED(m_pGameInstance->Change_Level(LoadingLevel)))
			return;

		LoadingLevel->Initialize();
	}
}

HRESULT CLogoLevel::Render()
{
	SetWindowText(g_hWnd, TEXT("로고레벨이빈다"));

	return S_OK;
}

void CLogoLevel::NextLevelChange(_bool flag)
{
	m_IsChangeLevel = flag;
	m_pGameInstance->Manager_StopSound(CHANNELID::BGM);
}

CLogoLevel* CLogoLevel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eID)
{
	CLogoLevel* pLogoLevel = new CLogoLevel(pDevice, pContext, ENUM_CLASS(eID));
	if(nullptr == pLogoLevel)
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
