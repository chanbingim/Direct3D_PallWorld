#include "FastTravelUI.h"

#include "GameInstance.h"
#include "FastTravelButton.h"

#include "GamePlayHUD.h"

#include "TerrainManager.h"
#include "PlayerManager.h"
#include "Player.h"

CFastTravelUI::CFastTravelUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CFastTravelUI::CFastTravelUI(const CFastTravelUI& rhs) :
    CBackGround(rhs)
{
}

HRESULT CFastTravelUI::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CFastTravelUI::Initialize(void* pArg)
{
    m_iZOrder = 3;
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CFastTravelUI::Update(_float fDeletaTime)
{
    if (VISIBILITY::HIDDEN == m_eVisible)
        return;

    for (auto& pButton : m_pButtons)
        pButton->Update(fDeletaTime);
}

void CFastTravelUI::Late_Update(_float fDeletaTime)
{
    if (VISIBILITY::HIDDEN == m_eVisible)
        return;

    for (auto& pButton : m_pButtons)
        pButton->Late_Update(fDeletaTime);

    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CFastTravelUI::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

HRESULT CFastTravelUI::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_BackGorund_FastTravel_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CFastTravelUI::ADD_Childs()
{
    // 버튼 4개
    m_pButtons.reserve(4);
    _float fStartPointY = m_pTransformCom->GetScale().y * 0.4f;
    CFastTravelButton::FAST_TRAVEL_DESC Desc = {};
    Desc.pParent = this;
    Desc.vScale = { 200, 40, 0.f };

    Desc.szMapName = TEXT("MainArea");
    Desc.szShowFontText = TEXT("영지");
    Desc.vPosition.y = -fStartPointY; //  +i * (Desc.vScale.y + 10.f);
    auto pButton = static_cast<CFastTravelButton*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Event_FastTravelButton"), &Desc));
    pButton->SetZOrder(m_iZOrder + 1);
    pButton->Bind_ClickEvent([&](const _wstring& MapName) { ButtonClickedEvent(MapName); });
    m_pButtons.push_back(pButton);

#pragma region Dororong
   
    Desc.szMapName = TEXT("SheepBalField");
    Desc.szShowFontText = TEXT("도로롱 서식지");
    Desc.vPosition.y = -fStartPointY + (Desc.vScale.y + 10.f);
    pButton = static_cast<CFastTravelButton*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Event_FastTravelButton"), &Desc));
    pButton->SetZOrder(m_iZOrder + 1);
    pButton->Bind_ClickEvent([&](const _wstring& MapName) { ButtonClickedEvent(MapName); });
    m_pButtons.push_back(pButton);
#pragma endregion

#pragma region PinkCat
    Desc.szMapName = TEXT("PinkCatField");
    Desc.szShowFontText = TEXT("배드냥 서식지");
    Desc.vPosition.y = -fStartPointY + 2 * (Desc.vScale.y + 10.f);
    pButton = static_cast<CFastTravelButton*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Event_FastTravelButton"), &Desc));
    pButton->SetZOrder(m_iZOrder + 1);
    pButton->Bind_ClickEvent([&](const _wstring& MapName) { ButtonClickedEvent(MapName); });
    m_pButtons.push_back(pButton);
#pragma endregion

#pragma region Electric Panda
    Desc.szMapName = TEXT("ElectPandaField");
    Desc.szShowFontText = TEXT("일렉판다 서식지");
    Desc.vPosition.y = -fStartPointY + 3 * (Desc.vScale.y + 10.f);
    pButton = static_cast<CFastTravelButton*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Event_FastTravelButton"), &Desc));
    pButton->SetZOrder(m_iZOrder + 1);
    pButton->Bind_ClickEvent([&](const _wstring& MapName) { ButtonClickedEvent(MapName); });
    m_pButtons.push_back(pButton);
#pragma endregion

#pragma region Boss Dungun
    Desc.szMapName = TEXT("BossField");
    Desc.szShowFontText = TEXT("보스 던전");
    Desc.vPosition.y = -fStartPointY + 4 * (Desc.vScale.y + 10.f);
    pButton = static_cast<CFastTravelButton*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Event_FastTravelButton"), &Desc));
    pButton->SetZOrder(m_iZOrder + 1);
    pButton->Bind_ClickEvent([&](const _wstring& MapName) { ButtonClickedEvent(MapName); });
    m_pButtons.push_back(pButton);
#pragma endregion

    return S_OK;
}

void CFastTravelUI::ButtonClickedEvent(const _wstring& MapName)
{
    //여기서 일단 이동이랑 UI 표현
    _float3 vMovePosition = {};
    auto pCurPlayer = CPlayerManager::GetInstance()->GetCurrentPlayer();
    if (pCurPlayer->GetPlayerOnChunkName() == MapName)
        return;

    auto pGamePlayHUD = static_cast<CGamePlayHUD*>(m_pGameInstance->GetCurrentHUD());
    pGamePlayHUD->UnActiveAllPopUp();

    // 다르면 이동 및 UI 제거
    if(CTerrainManager::GetInstance()->Find_FastTravelTransport(MapName.c_str(), &vMovePosition))
        pCurPlayer->TransportPlayer(vMovePosition);
}

CFastTravelUI* CFastTravelUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFastTravelUI* pFastTravel = new CFastTravelUI(pDevice, pContext);
    if (FAILED(pFastTravel->Initalize_Prototype()))
    {
        Safe_Release(pFastTravel);
        MSG_BOX("CREATE FAIL : FAST TRAVEl");
    }
    return pFastTravel;
}

CGameObject* CFastTravelUI::Clone(void* pArg)
{
    CFastTravelUI* pFastTravel = new CFastTravelUI(*this);
    if (FAILED(pFastTravel->Initialize(pArg)))
    {
        Safe_Release(pFastTravel);
        MSG_BOX("CLONE FAIL : FAST TRAVEl");
    }
    return pFastTravel;
}

void CFastTravelUI::Free()
{
    __super::Free();

    for (auto& pButton : m_pButtons)
        Safe_Release(pButton);

    m_pButtons.clear();
}
