#include "PalBoxSlot.h"

#include "GameInstance.h"
#include "ItemSlotIcon.h"

#include "PellBase.h"
#include "PlayerManager.h"
#include "PellBoxManager.h"

CPalBoxSlot::CPalBoxSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CPalBoxSlot::CPalBoxSlot(const CPalBoxSlot& rhs) :
    CBackGround(rhs)
{
}

HRESULT CPalBoxSlot::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPalBoxSlot::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    PAL_BOX_DESC* pPalSlotDesc = static_cast<PAL_BOX_DESC*>(pArg);
    m_ePalSlotType = pPalSlotDesc->ePalSlotType;

    m_eType = OBJECT_TYPE::STATIC;
    m_bIsMouseEvent = true;
    return S_OK;
}

void CPalBoxSlot::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);

    switch (m_ePalSlotType)
    {
    case PAL_SLOT_TYPE::INVEN:
    {
        auto pPellBase = CPlayerManager::GetInstance()->GetPellInfomation(m_iSlotNumber);
        if (pPellBase)
            m_pPalInfo = &pPellBase->GetPellInfo();
        else
            m_pPalInfo = nullptr;
    }
    break;
    case PAL_SLOT_TYPE::BOX:
        m_pPalInfo = CPellBoxManager::GetInstance()->GetPalBoxInfo(m_iSlotNumber);
        break;
    case PAL_SLOT_TYPE::WORK:
        m_pPalInfo = CPellBoxManager::GetInstance()->GetWorkPalInfo(m_iSlotNumber);
        break;
    }
}

void CPalBoxSlot::Late_Update(_float fDeletaTime)
{
    if (m_pPalInfo)
    {
        m_pSlotIcon->SetTexture(m_pPalInfo->pPellIconTexture);
        m_pSlotIcon->Late_Update(fDeletaTime);
    }
    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CPalBoxSlot::Render()
{
    if (FAILED(Apply_ConstantShaderResources()))
        return E_FAIL;

    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();
    return S_OK;
}

void CPalBoxSlot::SetPalSlotNumber(_uInt iSlotNumber)
{
    m_iSlotNumber = iSlotNumber;
   
}

void CPalBoxSlot::SwapSlot(CPalBoxSlot* From)
{
    if (nullptr == From->GetSlotPalInfo())
        return;

    const PAL_SLOT_TYPE FromType = From->GetPalSlotType();
    _uInt iFromSlotNumber = From->GetPalSlotNumber();
    if (m_ePalSlotType == FromType)
    {
        switch (m_ePalSlotType)
        {
        case PAL_SLOT_TYPE::INVEN:
            CPlayerManager::GetInstance()->SwapInventroyPal(iFromSlotNumber, m_iSlotNumber);
            break;
        case PAL_SLOT_TYPE::BOX:
        case PAL_SLOT_TYPE::WORK:
            CPellBoxManager::GetInstance()->SwapPalBox(ENUM_CLASS(m_ePalSlotType), m_iSlotNumber, iFromSlotNumber);
            break;
        }
    }
    else
    {
        _bool     bIsFromData{}, bIsToData{};
        PELL_INFO FromPalInfo{}, ToPalInfo{};

        bIsToData = LoadSlotPalInfo(m_ePalSlotType, m_iSlotNumber, &ToPalInfo);
        bIsFromData = LoadSlotPalInfo(FromType, iFromSlotNumber, &FromPalInfo);

        if (bIsFromData)
        {
            switch (m_ePalSlotType)
            {
            case PAL_SLOT_TYPE::INVEN:
                CPlayerManager::GetInstance()->ADDPellList(FromPalInfo, m_iSlotNumber);
                break;
            case PAL_SLOT_TYPE::BOX:
                CPellBoxManager::GetInstance()->StorePalBox(FromPalInfo, m_iSlotNumber);
                break;
            case PAL_SLOT_TYPE::WORK:
                CPellBoxManager::GetInstance()->Add_WorkPalList(FromPalInfo, m_iSlotNumber);
                break;
            }
        }

        if (bIsToData)
        {
            switch (FromType)
            {
            case PAL_SLOT_TYPE::INVEN:
                CPlayerManager::GetInstance()->ADDPellList(ToPalInfo, iFromSlotNumber);
                break;
            case PAL_SLOT_TYPE::BOX:
                CPellBoxManager::GetInstance()->StorePalBox(ToPalInfo, iFromSlotNumber);
                break;
            case PAL_SLOT_TYPE::WORK:
                CPellBoxManager::GetInstance()->Add_WorkPalList(ToPalInfo, iFromSlotNumber);
                break;
            }
        }
    }
}

void CPalBoxSlot::MouseHoverEnter()
{
}

void CPalBoxSlot::MouseHovering()
{
}

void CPalBoxSlot::MouseHoverExit()
{
}

void CPalBoxSlot::MouseButtonDwon()
{
    m_pGameInstance->SetMouseFocus(this);
}

void CPalBoxSlot::MouseButtonPressed()
{
}

void CPalBoxSlot::MouseButtonUp()
{
    if (m_bIsHover)
    {
        CUserInterface* pFoucusWidget = nullptr;
        CPalBoxSlot* pToSlot = nullptr;

        m_pGameInstance->GetMouseFocus((CUserInterface**)&pFoucusWidget);
        m_pGameInstance->SetDrag(false);
        auto pItemSlot = dynamic_cast<CPalBoxSlot*>(pFoucusWidget);
        if (pItemSlot)
            SwapSlot(pItemSlot);
    }
}

HRESULT CPalBoxSlot::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_PalSlot_Base_Texture"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    CItemSlotIcon::ITEM_SLOT_ICON_DESC pItemIconDesc = {};
    pItemIconDesc.pParent = this;
    pItemIconDesc.pParentTransform = m_pTransformCom;
    pItemIconDesc.vScale = m_pTransformCom->GetScale();
    pItemIconDesc.vScale.x -= 10.f;
    pItemIconDesc.vScale.y -= 10.f;
    pItemIconDesc.vScale.z -= 10.f;

    m_pSlotIcon = CItemSlotIcon::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pSlotIcon->Initialize(&pItemIconDesc);
    m_pSlotIcon->SetZOrder(m_iZOrder + 1);

    return S_OK;
}

_bool CPalBoxSlot::LoadSlotPalInfo(PAL_SLOT_TYPE eSlotType, _uInt SlotNumber, PELL_INFO* pOutPellInfo)
{
    _bool bFlag = false;
    switch (eSlotType)
    {
    case PAL_SLOT_TYPE::INVEN:
        bFlag = CPlayerManager::GetInstance()->LoadPellInfomation(SlotNumber, pOutPellInfo);
        break;
    case PAL_SLOT_TYPE::BOX:
        bFlag = CPellBoxManager::GetInstance()->LoadPalBox(SlotNumber, pOutPellInfo);
        break;
    case PAL_SLOT_TYPE::WORK:
        bFlag = CPellBoxManager::GetInstance()->Load_WorkPalList(SlotNumber, pOutPellInfo);
        break;
    }
    return bFlag;
}

CPalBoxSlot* CPalBoxSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPalBoxSlot* pPalBoxSlot = new CPalBoxSlot(pDevice, pContext);
    if (FAILED(pPalBoxSlot->Initalize_Prototype()))
    {
        Safe_Release(pPalBoxSlot);
        MSG_BOX("CREATE FAIL : Pal Box Slot");
    }
    return pPalBoxSlot;
}

CUserInterface* CPalBoxSlot::Clone(void* pArg)
{
    CPalBoxSlot* pPalBoxSlot = new CPalBoxSlot(*this);
    if (FAILED(pPalBoxSlot->Initialize(pArg)))
    {
        Safe_Release(pPalBoxSlot);
        MSG_BOX("CLONE FAIL : Pal Box Slot");
    }
    return pPalBoxSlot;
}

void CPalBoxSlot::Free()
{
    __super::Free();
    Safe_Release(m_pSlotIcon);
}
