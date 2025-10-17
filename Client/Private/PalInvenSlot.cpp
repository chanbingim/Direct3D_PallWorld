#include "PalInvenSlot.h"

#include "GameInstance.h"
#include "ItemSlotIcon.h"

#include "HealthBar.h"
#include "HungerBar.h"

CPalInvenSlot::CPalInvenSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : 
    CPalBoxSlot(pDevice, pContext)
{
}

CPalInvenSlot::CPalInvenSlot(const CPalInvenSlot& rhs) :
    CPalBoxSlot(rhs)
{
}

HRESULT CPalInvenSlot::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPalInvenSlot::Initialize(void* pArg)
{
    if (FAILED(CBackGround::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    PAL_BOX_DESC* pPalSlotDesc = static_cast<PAL_BOX_DESC*>(pArg);
    m_ePalSlotType = pPalSlotDesc->ePalSlotType;
    m_bIsMouseEvent = true;
    m_fAlpha = 1.f;
    m_fStartU = 0.3f;
    return S_OK;
}

void CPalInvenSlot::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);

    if (nullptr != m_pPalInfo)
    {
        m_PrintText = m_pPalInfo->NickName;
        m_pHealthBar->SetHealthBar(m_pPalInfo->CurHealth, m_pPalInfo->MaxHealth);
        m_pHungerBar->SetPercent(m_pPalInfo->CurHunger / m_pPalInfo->MaxHunger);
    }
    else
    {
        m_PrintText = TEXT("비어있음");
        m_pHealthBar->SetPercent(0);
        m_pHungerBar->SetPercent(0);
    }

    _vector vFontScale = m_pFontCom->GetFontBoundBox(m_PrintText.c_str());
    m_vNickNameFontPos = { (_float)GetRectSize().left + 10, (_float)GetRectSize().top };
    m_vNickNameFontPos.y += 3.f;
    
    for (auto& pChild : m_pChildList)
        pChild->Update(fDeletaTime);
}

void CPalInvenSlot::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);

    for (auto& pChild : m_pChildList)
        pChild->Late_Update(fDeletaTime);
}

HRESULT CPalInvenSlot::Render()
{
    if(FAILED(Bind_ButtonShader()))
        return E_FAIL;

    m_pShaderCom->Update_Shader(1);
    m_pVIBufferCom->Render_VIBuffer();

    if (FAILED(Apply_ConstantShaderResources()))
        return E_FAIL;

    m_pFrameShaderCom->Update_Shader(2);
    m_pFrameTex->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    m_pFontCom->Render(m_PrintText.c_str(), {1.f,1.f,1.f,1.f});
    return S_OK;
}

HRESULT CPalInvenSlot::Bind_ShaderResources()
{
    if (nullptr == m_pFrameShaderCom)
        return E_FAIL;

    m_pEMVWorldMat = m_pFrameShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pFrameShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pFrameShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();

    return S_OK;
}

HRESULT CPalInvenSlot::Bind_ButtonShader()
{
    m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->GetWorldMat());
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

    m_pShaderCom->Bind_SRV("g_Texture", m_pTextureCom->GetTexture(0));
    m_pShaderCom->Bind_SRV("g_EftTexture", m_pTextureCom->GetTexture(1));

    m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float));
    m_pShaderCom->Bind_RawValue("g_fStartU", &m_fStartU, sizeof(_float));
    return S_OK;
}

HRESULT CPalInvenSlot::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_PalSlot_Inven_Texture"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Button"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_PalSlot_Inven_Frame"), TEXT("FrameTexture_Com"), (CComponent**)&m_pFrameTex)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("FrameShader_Com"), (CComponent**)&m_pFrameShaderCom)))
        return E_FAIL;

    _float3 vScale = m_pTransformCom->GetScale();

#pragma region Prograss Bar
    CProgressBar::PROGRESS_DESC Desc = {};
    Desc.pParent = this;
    Desc.vScale = { 170.f, 20.f, 0.f };

    //Health bar
    Desc.vPosition = { -vScale.x * 0.5f + Desc.vScale.x * 0.5f + 10.f, (vScale.y * 0.5f) - 37.f, 0.f };
    Desc.vColor = { 1.f, 0.f, 0.f, 1.f };
    m_pHealthBar = static_cast<CHealthBar*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Health_Bar"), &Desc));
    m_pHealthBar->SetZOrder(m_iZOrder + 1);
    ADD_Child(m_pHealthBar);

    //Hunger bar
    Desc.vScale = { 170.f, 10.f, 0.f };
    Desc.vPosition = { -vScale.x * 0.5f + Desc.vScale.x * 0.5f + 10.f, (vScale.y * 0.5f) - 15.f, 0.f };
    Desc.vColor = { 250 / 255.f, 124 / 255.f, 35 / 255.f, 1.f };
    m_pHungerBar = static_cast<CHungerBar*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_GM_Hunger_Bar"), &Desc));
    m_pHungerBar->SetZOrder(m_iZOrder + 1);
    ADD_Child(m_pHungerBar);
#pragma endregion

#pragma region Slot ICon
    CItemSlotIcon::ITEM_SLOT_ICON_DESC pItemIconDesc = {};
    pItemIconDesc.pParent = this;
    pItemIconDesc.vScale = { vScale.y, vScale.y, 0.f };
    pItemIconDesc.vOffset = { vScale.x * 0.35f, 0.f, 0.f };
    pItemIconDesc.pParentTransform = m_pTransformCom;
    pItemIconDesc.vScale.x -= 5.f;
    pItemIconDesc.vScale.y -= 5.f;
    pItemIconDesc.vPosition = { vScale.x * 0.5f - vScale.y, 0.f, 0.f};

    m_pSlotIcon = CItemSlotIcon::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pSlotIcon->Initialize(&pItemIconDesc);
    m_pSlotIcon->SetZOrder(m_iZOrder + 1);
#pragma endregion

#pragma region Font
    // 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
    CFontComponent::FONT_DESC FontDesc = {};
   
    FontDesc.pPoint = &m_vNickNameFontPos;
    FontDesc.szUseFontName = TEXT("HanSanFont_16");
    FontDesc.vFontSize = { 50, 100 };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("Font_Com"), (CComponent**)&m_pFontCom, &FontDesc)))
        return E_FAIL;
#pragma endregion
    return S_OK;
}

CPalInvenSlot* CPalInvenSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPalInvenSlot* pPalInvenSlot = new CPalInvenSlot(pDevice, pContext);
    if(FAILED(pPalInvenSlot->Initalize_Prototype()))
    {
        Safe_Release(pPalInvenSlot);
        MSG_BOX("CREATE FAIL : PAL INVEN SLOT");
    }
    return pPalInvenSlot;
}

CUserInterface* CPalInvenSlot::Clone(void* pArg)
{
    CPalInvenSlot* pPalInvenSlot = new CPalInvenSlot(*this);
    if (FAILED(pPalInvenSlot->Initialize(pArg)))
    {
        Safe_Release(pPalInvenSlot);
        MSG_BOX("CLONE FAIL : PAL INVEN SLOT");
    }
    return pPalInvenSlot;
}

void CPalInvenSlot::Free()
{
    __super::Free();

    Safe_Release(m_pHealthBar);
    Safe_Release(m_pHungerBar);

    Safe_Release(m_pFontCom);
    Safe_Release(m_pFrameShaderCom);
    Safe_Release(m_pFrameTex);
}
