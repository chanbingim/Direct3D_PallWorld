#include "LoadingLevel.h"

#include "GameInstance.h"
#include "Loader.h"

#include "LogoLevel.h"
#include "GamePlayLevel.h"
#include "LoadingHUD.h"

#include "LoadingUI.h"
#include "LoadingIcon.h"
#include "PellLogo.h"

CLoadingLevel::CLoadingLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID, LEVEL eNextLevel) :
    CLevel(pDevice, pContext, _iID),
    m_eNextLevelID(eNextLevel)
{
}

HRESULT CLoadingLevel::Initialize()
{
    if (FAILED(Load_Data()))
        return E_FAIL;

    auto LoadingHUD = CLoadingHUD::Create(m_pGraphic_Device, m_pDeviceContext);
    SetHUD(LoadingHUD);
    if (FAILED(LoadingHUD->Initialize()))
        return E_FAIL;

    /* 다음 레벨에 대한 자원을 로드하여 준비해둔다. */
    m_pLoader = CLoader::Create(m_pGraphic_Device, m_pDeviceContext, m_eNextLevelID);
    if (nullptr == m_pLoader)
        return E_FAIL;

    return S_OK;
}

void CLoadingLevel::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    if (true == m_pLoader->isFinished())
    {
        /*if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_RETURN))
        {
            CLevel* pNewLevel = { nullptr };
            switch (m_eNextLevelID)
            {
            case LEVEL::LOGO:
                pNewLevel = CLogoLevel::Create(m_pGraphic_Device, m_pDeviceContext, m_eNextLevelID);
                break;
            case LEVEL::GAMEPLAY:
                pNewLevel = CGamePlayLevel::Create(m_pGraphic_Device, m_pDeviceContext, m_eNextLevelID);
                break;
            }

            if (FAILED(m_pGameInstance->Change_Level(pNewLevel)))
                return;

            pNewLevel->Initialize();
        }*/
       
        CLevel* pNewLevel = { nullptr };
        switch (m_eNextLevelID)
        {
        case LEVEL::LOGO:
            pNewLevel = CLogoLevel::Create(m_pGraphic_Device, m_pDeviceContext, m_eNextLevelID);
            break;
        case LEVEL::GAMEPLAY:
            pNewLevel = CGamePlayLevel::Create(m_pGraphic_Device, m_pDeviceContext, m_eNextLevelID);
            break;
        }

        if (FAILED(m_pGameInstance->Change_Level(pNewLevel)))
            return;

        pNewLevel->Initialize();
    }
}

HRESULT CLoadingLevel::Render()
{
    m_pLoader->Output();
    return S_OK;
}

_bool CLoadingLevel::IsFinishedLoader()
{
    if (nullptr == m_pLoader)
        return false;

    return m_pLoader->isFinished();
}

HRESULT CLoadingLevel::Load_Data()
{
    /* GAME_OBJECT_Loading_UI */
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOADING), TEXT("Prototype_GameObject_LoadingUI"), CLoadingUI::Create(m_pGraphic_Device, m_pDeviceContext))))
        return E_FAIL;

    /* GAME_OBJECT_Loading_UI */
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOADING), TEXT("Prototype_GameObject_LoadingIcon"), CLoadingIcon::Create(m_pGraphic_Device, m_pDeviceContext))))
        return E_FAIL;

    /* GAME_OBJECT_Loading_UI */
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOADING), TEXT("Prototype_GameObject_LoadingPellLogo"), CPellLogo::Create(m_pGraphic_Device, m_pDeviceContext))))
        return E_FAIL;

    return S_OK;
}

CLoadingLevel* CLoadingLevel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eID, LEVEL eNextLevel)
{
    CLoadingLevel* pLoadingLevel = new CLoadingLevel(pDevice, pContext, ENUM_CLASS(eID), eNextLevel);
    if (nullptr == pLoadingLevel)
    {
        Safe_Release(pLoadingLevel);
        MSG_BOX("CREATE FAIL : LOADING LEVEL");
    }
    return pLoadingLevel;
}

void CLoadingLevel::Free()
{
    __super::Free();

    Safe_Release(m_pLoader);
}
